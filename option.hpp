#ifndef ROC_OPTION_HPP
#define ROC_OPTION_HPP

#include <type_traits>
#include <initializer_list>
#include <ostream>

#include "utility.hpp"

namespace roc::tags
{
    struct prevent_init {};
    struct in_place {};
}

namespace roc
{
    namespace detail
    {
        struct valid_void_option_type {};

        template <typename T,
                  bool IsTriviallyDestructible = std::is_trivially_destructible<T>::value,
                  bool IsReference = std::is_reference<T>::value>
        struct option_storage {};

        template <typename T>
        struct option_storage<T, true, false>
        {
            constexpr option_storage() noexcept(std::is_nothrow_default_constructible<T>::value)
                : contains_value(false) {}

            template <typename... Args> requires std::is_constructible<T, Args&&...>::value
            constexpr option_storage(tags::in_place, Args&&... args) noexcept(
                    std::is_nothrow_constructible<T, Args&&...>::value)
                : stored_value(forward<Args>(args)...), contains_value(true) {}

            template <typename U, typename... Args> requires std::is_constructible<T, std::initializer_list<U>&, Args&&...>::value
            constexpr option_storage(tags::in_place, std::initializer_list<T> il, Args&&... args) noexcept(
                    std::is_nothrow_constructible<T, std::initializer_list<U>&, Args&&...>::value)
                : stored_value(il, forward<Args>(args)...), contains_value(true) {}

            ~option_storage() = default;

            T stored_value;
            bool contains_value;
        };

        template <typename T>
        struct option_storage<T, false, false>
        {
            constexpr option_storage() noexcept : contains_value(false) {}

            template <typename... Args> requires std::is_constructible<T, Args&&...>::value
            constexpr option_storage(tags::in_place, Args&&... args) noexcept(
                    std::is_nothrow_constructible<T, Args&&...>::value)
                : stored_value(forward<Args>(args)...), contains_value(true) {}

            template <typename U, typename... Args> requires std::is_constructible<T, std::initializer_list<U>&, Args&&...>::value
            constexpr option_storage(tags::in_place, std::initializer_list<T> il, Args&&... args) noexcept(
                    std::is_nothrow_constructible<T, std::initializer_list<U>&, Args&&...>::value)
                : stored_value(il, forward<Args>(args)...), contains_value(true) {}

            ~option_storage() {
                if (contains_value)
                    stored_value.~T();
            }

            T stored_value;
            bool contains_value;
        };

        template <>
        struct option_storage<void, false, false>
        {
            constexpr option_storage() noexcept : contains_value(true) {};
            constexpr option_storage(tags::prevent_init) noexcept : contains_value(false) {};
            ~option_storage() = default;
            
            bool contains_value;
        };

        template <typename T, bool Dummy>
        struct option_storage<T, Dummy, true>
        {
            const std::decay_t<T>* stored_pointer = nullptr;
        };


        template <typename T>
        struct option_opers : option_storage<T>
        {
            using option_storage<T>::option_storage;

            template <typename... Args> constexpr void construct(Args&&... args) noexcept
                requires(not std::is_reference<T>::value)
            {
                new(&(this->stored_value)) T(forward<Args>(args)...);
                this->contains_value = true;
            }
            template <typename V> constexpr void construct(V& target) noexcept
            {
                this->stored_pointer = &target;
            }

            template <typename Moved> constexpr void construct_with(Moved&& rhs) noexcept
                requires(not std::is_reference<T>::value)
            {
                new(&(this->stored_value)) T(forward<Moved>(rhs).get());
                this->contains_value = true;
            }
            constexpr void assign(const option_opers& rhs) noexcept
            {
                if (not this->contains_value && rhs->contains_value)
                {
                    if constexpr(std::is_nothrow_copy_constructible<T>::value) {
                        construct(move(rhs.get()));
                    }
                    else if constexpr(std::is_nothrow_move_constructible<T>::value) {
                        T tmp = rhs.get();
                        construct(move(tmp));
                    }
                    else
                    { 
                    #ifdef ROC_ENABLE_EXCEPTIONS
                        try {
                            construct(rhs.get());
                        } catch (...) {
                            this->contains_value = false;
                            throw;
                        }
                    #else
                        static_assert(std::is_nothrow_move_constructible<T>::value
                                   || std::is_nothrow_copy_constructible<T>::value,
                                   "Cannot keep nothrow-guarantee");
                    #endif
                    }
                } else {
                    assign_common(rhs);
                }
            };

            constexpr void assign(option_opers&& rhs) noexcept
            {
                if (not this->contains_value && rhs->contains_value)
                {
                    if constexpr(std::is_nothrow_move_constructible<T>::value) {
                        construct(move(rhs).get());
                    } else {
                    #ifdef ROC_ENABLE_EXCEPTIONS
                        try {
                            construct(move(rhs.get()));
                        } catch (...) {
                            this->contains_value = false;
                            throw;
                        }
                    #else
                        static_assert(std::is_nothrow_move_constructible<T>::value
                                   || std::is_nothrow_copy_constructible<T>::value,
                                   "Cannot keep nothrow-guarantee");
                    #endif
                    }
                } else {
                    assign_common(move(rhs));
                }
            }

            template <typename Rhs>
            constexpr void assign_common(Rhs&& rhs) noexcept
            {
                if (not this->contains_value && not rhs->contains_value) { /* noop */ }
                else if (rhs->contains_value)
                {
                    get() = forward<Rhs>(rhs).get();
                } else {
                    destroy_value();
                    this->contains_value = false;
                }
            }

            constexpr bool has_value() const requires (not std::is_reference<T>::value)
                { return this->contains_value; }

            constexpr bool has_value() const requires (std::is_reference<T>::value)
                { return this->stored_pointer != nullptr; }

            constexpr T& get() & { 
                if constexpr (std::is_reference<T>::value)
                    return *(this->stored_pointer);
                else
                    this->stored_value;
            }

            constexpr const T& get() const & {
                if constexpr (std::is_reference<T>::value)
                    return const_cast<const T&>(*(this->stored_pointer));
                else
                    return this->stored_value;
            }

            constexpr T&& get() && { 
                if constexpr (std::is_reference<T>::value)
                    return move(*(this->stored_pointer));
                else
                    return move(this->stored_value);
            }

            constexpr const T&& get() const && {
                // this doesn't make any sense?
                if constexpr (std::is_reference<T>::value)
                    return const_cast<const T&&>(move(*(this->stored_pointer)));
                else
                    return move(this->stored_value);
            }

            constexpr void destroy_value() { get().~T(); }
        };

        template <>
        struct option_opers<void> : option_storage<void>
        {
            template <typename... Args> constexpr void construct() noexcept { this->contains_value = true; }
            template <typename Rhs> constexpr void construct_with(Rhs&&) noexcept { this->contains_value = true; }

            constexpr void assign(valid_void_option_type) noexcept {
                this->contains_value = true;
            }

            constexpr void assign(none_type) noexcept {
                this->contains_value = false;
            }

            constexpr bool has_value() const { return this->contains_value; }
        };
    }

    template <typename T, bool IsReference = std::is_reference_v<T>>
    struct option : detail::option_opers<T>
    {
        constexpr option() = default;
        constexpr option(none_type) noexcept { this->contains_value = false; }

        template <typename... Args>
        explicit constexpr option(Args&&... args) noexcept { this->construct(forward<Args...>(args...)); }

        constexpr bool is_some() const noexcept { return this->has_value(); } 
        constexpr bool is_none() const noexcept { return !this->has_value(); }

        template <typename U>
        constexpr bool contains(U&& compare) const noexcept { return is_some() && this->stored_value == compare; }

        constexpr const T& unwrap() const & {
            if (is_none()) THROW_OR_PANIC(); else return this->get();
        }
        constexpr T& unwrap() & {
            if (is_none()) THROW_OR_PANIC(); else return this->get();
        }

        constexpr T&& unwrap() && {
            if (is_none()) THROW_OR_PANIC(); else return this->get();
        }
        constexpr const T&& unwrap() const&& {
            if (is_none()) THROW_OR_PANIC(); else return this->get();
        }

        template <typename U> requires (std::is_copy_constructible<T>::value && std::is_convertible<U&&, T>::value)
        constexpr const T& unwrap_or(U&& v) const & {
            return is_some()? unwrap() : static_cast<T>(forward<U>(v));
        }
        template <typename U> requires (std::is_copy_constructible<T>::value && std::is_convertible<U&&, T>::value)
        constexpr T& unwrap_or(U&& v) & {
            return is_some()? unwrap() : static_cast<T>(forward<U>(v));
        }

        template <typename U> requires (std::is_copy_constructible<T>::value && std::is_convertible<U&&, T>::value)
        constexpr const T&& unwrap_or(U&& v) const && {
            return is_some()? move(unwrap()) : static_cast<T>(forward<U>(v));
        }
        template <typename U> requires (std::is_copy_constructible<T>::value && std::is_convertible<U&&, T>::value)
        constexpr T&& unwrap_or(U&& v) && {
            return is_some()? move(unwrap()) : static_cast<T>(forward<U>(v));
        }
    };

    template <typename T>
    struct option<T, true> : detail::option_opers<T>
    {
        constexpr option() = default;
        constexpr option(none_type) noexcept { this->contains_value = false; }
        constexpr option(const option&) = default;
        constexpr option(option&&) = default;

        template <typename... Args>
        explicit constexpr option(Args&&... args) noexcept { this->construct(forward<Args...>(args...)); }

        constexpr option& operator=(const option&) = delete;
        constexpr option&& operator=(option&&) = delete;

        constexpr bool is_some() const noexcept { return this->has_value(); } 
        constexpr bool is_none() const noexcept { return !this->has_value(); }

        template <typename U>
        constexpr bool contains(U&& compare) const noexcept { return is_some() && this->stored_value == compare; }

        constexpr const T& unwrap() const & {
            if (is_none()) THROW_OR_PANIC(); else return this->get();
        }
        constexpr T& unwrap() & {
            if (is_none()) THROW_OR_PANIC(); else return this->get();
        }

        constexpr T&& unwrap() && {
            if (is_none()) THROW_OR_PANIC(); else return this->get();
        }
        constexpr const T&& unwrap() const&& {
            if (is_none()) THROW_OR_PANIC(); else return this->get();
        }

        template <typename U> requires (std::is_copy_constructible<T>::value && std::is_convertible<U&&, T>::value)
        constexpr const T& unwrap_or(U&& v) const & {
            return is_some()? unwrap() : static_cast<T>(forward<U>(v));
        }
        template <typename U> requires (std::is_copy_constructible<T>::value && std::is_convertible<U&&, T>::value)
        constexpr T& unwrap_or(U&& v) & {
            return is_some()? unwrap() : static_cast<T>(forward<U>(v));
        }

        template <typename U> requires (std::is_copy_constructible<T>::value && std::is_convertible<U&&, T>::value)
        constexpr const T&& unwrap_or(U&& v) const && {
            return is_some()? move(unwrap()) : static_cast<T>(forward<U>(v));
        }
        template <typename U> requires (std::is_copy_constructible<T>::value && std::is_convertible<U&&, T>::value)
        constexpr T&& unwrap_or(U&& v) && {
            return is_some()? move(unwrap()) : static_cast<T>(forward<U>(v));
        }
    };

    template <>
    struct option<void> : detail::option_opers<void>
    {
        constexpr option() = default;
        constexpr option(none_type) noexcept { this->contains_value = false; }
        constexpr option(detail::valid_void_option_type) noexcept { this->contains_value = true; }

        constexpr bool is_some() const noexcept { return this->contains_value; }
        constexpr bool is_none() const noexcept { return !this->contains_value; }
    };
    
    template <typename T>
    std::ostream& operator<<(std::ostream& stream, const option<T>& opt) {
        if (opt.is_some()) {
            if constexpr (std::is_void<T>::value)
                stream << "Some()";
            else
                stream << "Some(" << opt.unwrap() << ")";
        } else {
            stream << "None";
        }
        return stream;
    }
}

namespace roc::import
{
    template <typename T>
    inline constexpr option<T> Some(T&& t) {
        return option<T>{forward<T>(t)};
    }

    inline constexpr option<void> Some() noexcept {
        return option<void>{detail::valid_void_option_type{}};
    }

    constexpr static none_type None {};
}

#endif
