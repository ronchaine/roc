#ifndef ROC_OPTION_HPP
#define ROC_OPTION_HPP

#include <type_traits>
#include <initializer_list>

#if defined (ROC_ENABLE_STD_STREAMS)
#include <ostream>
#endif
#if defined (ROC_ENABLE_EXCEPTIONS)
#include <exception>
namespace roc {
    struct bad_option_access : public std::exception
    {
        bad_option_access() = default;
        const char *what() const noexcept override { return "Optional has no value"; }
    };
}
#endif

#include "utility.hpp"
#include "monadic.hpp"

namespace roc
{
    namespace detail
    {
        template <typename T,
                  bool IsTriviallyDestructible = std::is_trivially_destructible<T>::value,
                  bool IsReference = std::is_reference<T>::value>
        struct option_storage {};

        template <typename T>
        struct option_storage<T, TRIVIALLY_DESTRUCTIBLE_VALUE, NOT_REFERENCE>
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
        struct option_storage<T, NOT_TRIVIALLY_DESTRUCTIBLE_VALUE, NOT_REFERENCE>
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
        struct option_storage<void, NOT_TRIVIALLY_DESTRUCTIBLE_VALUE, NOT_REFERENCE>
        {
            constexpr option_storage() noexcept : contains_value(true) {};
            constexpr option_storage(tags::prevent_init) noexcept : contains_value(false) {};
            ~option_storage() = default;
            
            bool contains_value;
        };

        template <typename T, bool DONT_CARE>
        struct option_storage<T, DONT_CARE, IS_REFERENCE>
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

            constexpr bool has_value() const requires (not std::is_reference<T>::value)
                { return this->contains_value; }

            constexpr bool has_value() const requires (std::is_reference<T>::value)
                { return this->stored_pointer != nullptr; }

            constexpr T& get() & { 
                if constexpr (std::is_reference<T>::value)
                    return *(this->stored_pointer);
                else
                    return this->stored_value;
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

            constexpr bool has_value() const { return this->contains_value; }
        };
    }

    template <typename T, typename = boolopt<std::is_reference_v<T>>>
    struct option : detail::option_opers<T>
    {
        using value_type = T;

        constexpr option() = default;
        constexpr option(none_type) noexcept { this->contains_value = false; }

        template <typename... Args>
        explicit constexpr option(Args&&... args) noexcept { this->construct(forward<Args...>(args...)); }

        constexpr bool is_some() const noexcept { return this->has_value(); } 
        constexpr bool is_none() const noexcept { return !this->has_value(); }

        template <typename U>
        constexpr bool contains(U&& compare) const noexcept { return is_some() && this->stored_value == compare; }

        constexpr const T& unwrap() const & {
            if (is_none()) THROW_OR_PANIC(bad_option_access()); else return this->get();
        }
        constexpr T& unwrap() & {
            if (is_none()) THROW_OR_PANIC(bad_option_access()); else return this->get();
        }

        constexpr T&& unwrap() && {
            if (is_none()) THROW_OR_PANIC(bad_option_access()); else return move(this->get());
        }
        constexpr const T&& unwrap() const&& {
            if (is_none()) THROW_OR_PANIC(bad_option_access()); else return move(this->get());
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

        template <typename Func>
        constexpr auto and_then(Func&& f) {
            using result_type = typename std::invoke_result<Func, value_type&>::type;
            return is_some()? f(unwrap())
                : result_type{none_type{}};
        }
    };

    template <typename T>
    struct option<T, boolopt<true>> : detail::option_opers<T>
    {
        using value_type = T;

        constexpr option() = default;
        constexpr option(none_type) noexcept { this->contains_value = false; }
        constexpr option(const option&) = default;
        constexpr option(option&&) = default;

        template <typename... Args>
        explicit constexpr option(Args&&... args) noexcept { this->construct(forward<Args...>(args...)); }

        // The comments here are obvious, but the point is to show the comment line
        // with the error message when trying to do this in user program
        constexpr option& operator=(const option&) = delete; // do not allow rebinding a reference
        constexpr option&& operator=(option&&) = delete; // do not allow rebinding a reference

        constexpr bool is_some() const noexcept { return this->has_value(); } 
        constexpr bool is_none() const noexcept { return !this->has_value(); }

        template <typename U>
        constexpr bool contains(U&& compare) const noexcept { return is_some() && this->stored_value == compare; }

        option& rebind(T&& t) && { this->construct(t); return *this; }
        option& rebind(T&& t) & { this->construct(t); return *this; }

        constexpr const T& unwrap() const & {
            if (is_none()) THROW_OR_PANIC(bad_option_access()); else return this->get();
        }
        constexpr T& unwrap() & {
            if (is_none()) THROW_OR_PANIC(bad_option_access()); else return this->get();
        }

        constexpr T&& unwrap() && {
            if (is_none()) THROW_OR_PANIC(bad_option_access()); else return move(this->get());
        }
        constexpr const T&& unwrap() const&& {
            if (is_none()) THROW_OR_PANIC(bad_option_access()); else return move(this->get());
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

        template <typename Func>
        constexpr auto and_then(Func&& f) {
            using result_type = typename std::invoke_result<Func, value_type&>::type;
            return is_some()? f(unwrap())
                : result_type{none_type{}};
        }
        template <typename Func>
        constexpr auto map(Func&& f) {
            using result_value_type = typename std::invoke_result<Func, value_type>::type::value_type;
            return is_some()? option<result_value_type>{f(unwrap())}
                : option<result_value_type>{this->geterr()};
        }

        template <typename Func>
        constexpr auto map_err(Func&& f) {
            using result_value_type = typename std::invoke_result<Func, value_type>::type::value_type;
            return is_some()? option<result_value_type>{unwrap()}
                : option<result_value_type>{f(this->geterr())};
        }

        template <typename Func>
        constexpr auto or_else(Func&& f) {
            return is_some()? *this
                : option<T>{f(this->geterr())};
        }
    };

    template <>
    struct option<void> : detail::option_opers<void>
    {
        constexpr option() = default;
        constexpr option(none_type) noexcept { this->contains_value = false; }
        constexpr option(valid_void_type) noexcept { this->contains_value = true; }

        constexpr bool is_some() const noexcept { return this->contains_value; }
        constexpr bool is_none() const noexcept { return !this->contains_value; }
    };
    
    #if defined (ROC_ENABLE_STD_STREAMS)
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
    #endif
}

namespace roc::import
{
    template <typename T>
    inline constexpr option<T> Some(T&& t) {
        return option<T>{forward<T>(t)};
    }

    inline constexpr option<void> Some() noexcept {
        return option<void>{valid_void_type{}};
    }

    constexpr static none_type None {};
}

namespace roc
{
    template <>
    struct monad_bind<option>
    {
        template <typename T,
                  roc::callable Func,
                  typename R = typename std::invoke_result<Func, T>::type>
        constexpr static R bind(const option<T>& opt, Func&& f)
        {
            return opt.and_then(f);
        }
    };
}

#endif
