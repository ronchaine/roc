#ifndef ROC_RESULT_HPP
#define ROC_RESULT_HPP

#include <initializer_list>
#include <compare>

#if defined (ROC_ENABLE_STD_STREAMS)
#include <ostream>
#endif
#if defined (ROC_ENABLE_EXCEPTIONS)
# include <exception>
namespace roc {
    struct bad_result_access : public std::exception
    {
        bad_result_access() = default;
        const char* what() const noexcept override { return "Bad result access"; } 
    };
}
#endif

#include "utility.hpp"
#include "monadic.hpp"

namespace roc
{
    template <typename T, bool = std::is_reference<T>::value>
    class success_type
    {
        public:
            explicit constexpr success_type(const T& val) noexcept(std::is_nothrow_copy_constructible<T>::value)
                : expected_value(val) {}
            explicit constexpr success_type(T&& val) noexcept(std::is_nothrow_move_constructible<T>::value)
                : expected_value(move(val)) {}

            template <typename... Args> requires std::is_constructible<T, Args&&...>::value
            explicit constexpr success_type(Args... args) noexcept(std::is_nothrow_constructible<T, Args&&...>::value)
                : expected_value(forward<Args>(args)...) {}

            explicit constexpr operator T() const & noexcept { return expected_value; }
            explicit constexpr operator T() const && noexcept { return ::roc::move(expected_value); }
            explicit constexpr operator T() & noexcept { return expected_value; }
            explicit constexpr operator T() && noexcept { return ::roc::move(expected_value); }

            constexpr T value() const & noexcept { return expected_value; }
            constexpr T value() const && noexcept { return ::roc::move(expected_value); }
            constexpr T value() & noexcept { return expected_value; }
            constexpr T value() && noexcept { return ::roc::move(expected_value); }

            friend auto operator<=>(const success_type&, const success_type&) noexcept = default;

        private:
            T expected_value{};
    };

    template <typename T>
    class success_type<T, detail::IS_REFERENCE>
    {
        static_assert(std::is_lvalue_reference<T>::value);
        public:
            using plain_type = std::decay_t<T>;
            using reference_type = plain_type&;

            explicit constexpr success_type(const plain_type& val) noexcept : expected_ptr(const_cast<plain_type*>(&val)) {}
            constexpr success_type(const success_type<plain_type, false>& ref) noexcept : expected_ptr(&(ref.value())) {}

            constexpr reference_type value() const & noexcept { return *expected_ptr; }
            constexpr reference_type value() const && noexcept { return *expected_ptr; }
            constexpr reference_type value() & noexcept { return *expected_ptr; }
            constexpr reference_type value() && noexcept { return *expected_ptr; }

        private:
            plain_type* const expected_ptr;
    };

    template <> class success_type<void> {};

    template <typename E> requires (not std::is_void<E>::value && not std::is_reference<E>::value)
    class error_type
    {
        public:
            using type = E;

            explicit constexpr error_type(const E& err) noexcept(std::is_nothrow_copy_constructible<E>::value) 
                : unexpected_value(err) {}
            explicit constexpr error_type(E&& err) noexcept(std::is_nothrow_move_constructible<E>::value)
                : unexpected_value(move(err)) {}

            template <typename... Args> requires std::is_constructible<E, Args&&...>::value
            explicit constexpr error_type(Args... args) noexcept(std::is_nothrow_constructible<E, Args&&...>::value)
                : unexpected_value(forward<Args>(args)...) {}

            explicit constexpr operator E() const & noexcept { return unexpected_value; }
            explicit constexpr operator E() const && noexcept { return unexpected_value; }
            explicit constexpr operator E() & noexcept { return unexpected_value; }
            explicit constexpr operator E() && noexcept { return unexpected_value; }

            error_type() = delete;

            friend auto operator<=>(const error_type&, const error_type&) noexcept = default;

        private:
            E unexpected_value;
    };

    namespace detail
    {
        template <typename T,
                  typename E,
                  bool IsReference = std::is_reference<T>::value,
                  bool TriviallyDestructibleT = std::is_trivially_destructible<T>::value,
                  bool TriviallyDestructibleE = std::is_trivially_destructible<E>::value>
        struct result_storage {};

        template <typename T, typename E>
        struct result_storage<T, E, NOT_REFERENCE, TRIVIALLY_DESTRUCTIBLE_VALUE, TRIVIALLY_DESTRUCTIBLE_ERROR>
        {
            constexpr result_storage() noexcept : uninitialised(), contains_value(false) {}
            constexpr result_storage(const T& value) requires (std::is_trivially_copy_constructible<T>::value)
                : stored_value(value), contains_value(true) {}
            constexpr result_storage(T&& value) requires (std::is_trivially_move_constructible<T>::value)
                : stored_value(move(value)), contains_value(true) {}
            constexpr result_storage(const error_type<E>& error) requires (std::is_trivially_copy_constructible<error_type<E>>::value)
                : stored_error(error), contains_value(false) {}
            constexpr result_storage(error_type<E>&& error) requires (std::is_trivially_move_constructible<error_type<E>>::value)
                : stored_error(move(error)), contains_value(false) {}

            constexpr result_storage(const result_storage&) noexcept = default;
            constexpr result_storage(result_storage&&) noexcept = default;
            constexpr result_storage& operator=(const result_storage&) noexcept = default;
            constexpr result_storage& operator=(result_storage&&) noexcept = default;

            ~result_storage() = default;

            union {
                T               stored_value;
                error_type<E>   stored_error;
                char            uninitialised;
            };
            bool contains_value;
        };

        template <typename T, typename E,
                  bool TriviallyDestructibleT,
                  bool TriviallyDestructibleE>
        struct result_storage<T, E, NOT_REFERENCE, TriviallyDestructibleT, TriviallyDestructibleE>
        {
            constexpr result_storage() noexcept : uninitialised(), contains_value(false) {}
            constexpr result_storage(const T& value) requires (std::is_trivially_copy_constructible<T>::value)
                : stored_value(value), contains_value(true) {}
            constexpr result_storage(T&& value) requires (std::is_trivially_move_constructible<T>::value)
                : stored_value(move(value)), contains_value(true) {}
            constexpr result_storage(const error_type<E>& error) requires (std::is_trivially_copy_constructible<error_type<E>>::value)
                : stored_error(error), contains_value(false) {}
            constexpr result_storage(error_type<E>&& error) requires (std::is_trivially_move_constructible<error_type<E>>::value)
                : stored_error(move(error)), contains_value(false) {}

            constexpr result_storage(const result_storage&) noexcept = default;
            constexpr result_storage(result_storage&&) noexcept = default;
            constexpr result_storage& operator=(const result_storage&) noexcept = default;
            constexpr result_storage& operator=(result_storage&&) noexcept = default;

            ~result_storage() {
                if constexpr (not TriviallyDestructibleT)
                    if (contains_value)
                        stored_value.~T();
                if constexpr (not TriviallyDestructibleE)
                    if (not contains_value)
                        stored_error.~error_type<E>();
            }

            union {
                T               stored_value;
                error_type<E>   stored_error;
                char            uninitialised;
            };
            bool contains_value;
        };

        template <typename T, typename E>
        struct result_storage<T, E, IS_REFERENCE, TRIVIALLY_DESTRUCTIBLE_VALUE, TRIVIALLY_DESTRUCTIBLE_ERROR>
        {
            static_assert(not std::is_void_v<T>);
            constexpr result_storage() noexcept : uninitialised(), contains_value(false) {}
            constexpr result_storage(T&& value) : stored_pointer(&value), contains_value(true) {}
            constexpr result_storage(const error_type<E>& error) : stored_error(error), contains_value(false) {}
            constexpr result_storage(error_type<E>&& error) : stored_error(move(error)), contains_value(false) {}

            constexpr result_storage(const result_storage&) = default;
            constexpr result_storage(result_storage&&) = default;

            ~result_storage() = default;

            union {
                std::decay_t<T>* const  stored_pointer;
                error_type<E>           stored_error;
                char                    uninitialised;
            };
            bool contains_value;
        };

        template <typename E>
        struct result_storage<void, E, NOT_REFERENCE, false, TRIVIALLY_DESTRUCTIBLE_ERROR>
        {
            struct dummy {};

            constexpr result_storage() noexcept : dummy_value(), contains_value(false) {}
            constexpr result_storage(success_type<void>&&) : dummy_value(), contains_value(true) {}
            constexpr result_storage(const error_type<E>& error) : stored_error(error), contains_value(false) {}
            constexpr result_storage(error_type<E>&& error) : stored_error(move(error)), contains_value(false) {}

            union {
                dummy           dummy_value;
                error_type<E>   stored_error;
            };
            bool contains_value = false;
        };

        template <typename E>
        struct result_storage<void, E, NOT_REFERENCE, false, NOT_TRIVIALLY_DESTRUCTIBLE_ERROR>
        {
            struct dummy {};

            constexpr result_storage() noexcept : dummy_value(), contains_value(false) {}
            constexpr result_storage(success_type<void>&&) : dummy_value(), contains_value(true) {}
            constexpr result_storage(const error_type<E>& error) : stored_error(error), contains_value(false) {}
            constexpr result_storage(error_type<E>&& error) : stored_error(move(error)), contains_value(false) {}

            ~result_storage() {
                if (not contains_value)
                    stored_error.~error_type<E>();
            }

            union {
                dummy           dummy_value;
                error_type<E>   stored_error;
            };
            bool contains_value = false;
        };

        template <typename T, typename E>
        struct result_storage_adds : result_storage<T, E>
        {
            using result_storage<T, E>::result_storage;

            constexpr result_storage_adds() noexcept = default;
            constexpr result_storage_adds(const result_storage_adds&) noexcept = default;

            constexpr result_storage_adds(const T& v) noexcept : result_storage<T, E>(v) {}
            constexpr result_storage_adds(T&& v) noexcept requires (not std::is_reference<T>::value)
                : result_storage<T, E>(move(v)){}

            constexpr result_storage_adds(const error_type<E>& error) : result_storage<T, E>(error) {}
            constexpr result_storage_adds(error_type<E>&& error) : result_storage<T, E>(error) {}

            template <typename... Args> constexpr void construct(Args&&... args) noexcept
                requires (not std::is_reference<T>::value)
            {
                new(&(this->stored_value)) T(::roc::forward<Args>(args)...);
                this->contains_value = true;
            }
            template <typename Moved> constexpr void construct_with(Moved&& rhs) noexcept
                requires (not std::is_reference<T>::value)
            {
                new(&(this->stored_value)) T(forward<Moved>(rhs).get());
                this->contians_value = true;
            }
            template <typename... Args> constexpr void construct_error(Args&&... args) noexcept
            {
                new(&(this->stored_error)) error_type<E>(forward<Args>(args)...);
                this->contains_value = false;
            }

            constexpr bool has_value() const { return this->contains_value; }

            constexpr T& get() & {
                if (this->contains_value) {
                    if constexpr (std::is_reference<T>::value)
                        return *(this->stored_pointer);
                    else
                        return this->stored_value;
                }
                THROW_OR_PANIC(bad_result_access());
            }
            constexpr const T& get() const & {
                if (this->contains_value) {
                    if constexpr (std::is_reference<T>::value)
                        return const_cast<const T&>(*(this->stored_pointer));
                    else
                        return this->stored_value;
                }
                THROW_OR_PANIC(bad_result_access());
            }
            constexpr T&& get() && {
                if (this->contains_value) {
                    if constexpr (std::is_reference<T>::value)
                        return move(*(this->stored_pointer));
                    else
                        return move(this->stored_value);
                }
                THROW_OR_PANIC(bad_result_access());
            }
            constexpr const T&& get() const && {
                if (this->contains_value) {
                    if constexpr (std::is_reference<T>::value)
                        // this doesn't make sense?
                        return const_cast<const T&>(move(*(this->stored_pointer)));
                    else
                        return move(this->stored_value);
                }
                THROW_OR_PANIC(bad_result_access());
            }

            constexpr error_type<E>& geterr() & { return this->stored_error; }
            constexpr const error_type<E>& geterr() const& { return this->stored_error; }
            constexpr error_type<E>&& geterr() && { return move(this->stored_error); }
            constexpr const error_type<E>&& geterr() const&& { return move(this->stored_error); }

            constexpr void destroy_value() { get().~T(); }
        };

        template <typename E>
        struct result_storage_adds<void, E> : result_storage<void, E>
        {
            using result_storage<void, E>::result_storage;

            template <typename... Args>
            constexpr void construct() noexcept { this->contains_value = true; }
            template <typename Rhs>
            constexpr void construct_with(Rhs&&) noexcept { this->contains_value = true; }

            template <typename... Args>
            constexpr void construct_error(Args&&... args) noexcept
            {
                new(&(this->stored_error)) error_type<E>(forward<Args>(args)...);
                this->contains_value = false;
            }

            constexpr bool has_value() const { return this->contains_value; }

            constexpr error_type<E>& geterr() & { return this->stored_error; }
            constexpr const error_type<E>& geterr() const& { return this->stored_error; }
            constexpr error_type<E>&& geterr() && { return move(this->stored_error); }
            constexpr const error_type<E>&& geterr() const&& { return move(this->stored_error); }

            constexpr void destroy_value() {}
        };
    }

    template <typename T, typename E>
    struct result : detail::result_storage_adds<T, E>
    {
        static_assert(not std::is_reference<E>::value, "error type cannot be a reference");
        public:
            using value_type = T;
            using unexpected_type = error_type<E>;

            constexpr result() noexcept = default;
            constexpr result(const result&) noexcept(
                    std::is_nothrow_copy_constructible<T>::value
                 && std::is_nothrow_copy_constructible<unexpected_type>::value) = default;

            constexpr result(result&&) noexcept(
                    std::is_nothrow_move_constructible<T>::value
                 && std::is_nothrow_move_constructible<unexpected_type>::value) = default;

            constexpr result& operator=(const result&) noexcept = default;
            constexpr result& operator=(result&&) noexcept = default;

            template <typename U> requires (std::is_convertible<U&&, T>::value && (not std::is_reference<T>::value))
            constexpr result(success_type<U>&& v) noexcept(std::is_nothrow_convertible<U&&, T>::value) {
                this->construct(static_cast<T>(::roc::forward<success_type<U>>(v).value()));
            }

            template <typename U> requires (std::is_reference<T>::value)
            constexpr result(success_type<U>&& v) noexcept : detail::result_storage_adds<T, E>(v.value()) {}

            template <typename U> requires (std::is_convertible<U&&, E>::value)
            constexpr result(error_type<U>&& v) noexcept(std::is_nothrow_convertible<U&&, E>::value) {
                this->construct_error(static_cast<E>(::roc::forward<error_type<U>>(v)));
            }

            constexpr result& operator=(success_type<T>&& value) noexcept { this->construct(value); return *this; }
            constexpr result& operator=(error_type<E>&& error) noexcept { this->construct_error(error); return *this; };

            constexpr bool is_ok() const noexcept { return this->has_value(); }
            constexpr bool is_err() const noexcept { return !this->has_value(); }

            constexpr bool contains(const std::decay_t<T>& t) const noexcept { return is_ok()? t == unwrap() : false; }
            constexpr bool contains_err(const E&& e) const noexcept { return is_err()? e == static_cast<E>(this->geterr()) : false; }

            constexpr const T& unwrap() const & {
                if (is_err()) THROW_OR_PANIC(bad_result_access()); else return this->get();
            }
            constexpr T& unwrap() & {
                if (is_err()) THROW_OR_PANIC(bad_result_access()); else return this->get();
            }
            constexpr const T&& unwrap() const && {
                if (is_err()) THROW_OR_PANIC(bad_result_access()); else return ::roc::move(this->get());
            }
            constexpr T&& unwrap() && {
                if (is_err()) THROW_OR_PANIC(bad_result_access()); else return ::roc::move(this->get());
            }

            constexpr const E& err_value() const & {
                if (is_ok()) THROW_OR_PANIC(bad_result_access()); else return this->geterr();
            }
            constexpr E& err_value() & {
                if (is_ok()) THROW_OR_PANIC(bad_result_access()); else return this->geterr();
            }
            constexpr const E&& err_value() const && {
                if (is_ok()) THROW_OR_PANIC(bad_result_access()); else return ::roc::move(this->geterr());
            }
            constexpr E&& err_value() && {
                if (is_ok()) THROW_OR_PANIC(bad_result_access()); else return ::roc::move(this->geterr());
            }

            template <typename U> requires (std::is_copy_constructible<T>::value && std::is_convertible<U&&, T>::value)
            constexpr T unwrap_or(U&& v) const& noexcept(std::is_nothrow_convertible<U&&, T>::value) {
                return is_ok()? unwrap() : static_cast<T>(forward<U>(v));
            }
            template <typename U> requires (std::is_copy_constructible<T>::value && std::is_convertible<U&&, T>::value)
            constexpr T unwrap_or(U&& v) && noexcept(std::is_nothrow_convertible<U&&, T>::value) {
                return is_ok()? move(unwrap()) : static_cast<T>(forward<U>(v));
            }

            template <typename Func>
            constexpr auto and_then(Func&& f) {
                using result_type = typename std::invoke_result<Func, value_type>::type;
                return is_ok()? f(unwrap()) : result_type{this->geterr()};
            }

            template <typename Func>
            constexpr auto map(Func&& f) {
                using result_value_type = typename std::invoke_result<Func, value_type>::type::value_type;
                return is_ok()? result<result_value_type, E>{f(unwrap())}
                    : result<result_value_type, E>{this->geterr()};
            }

            template <typename Func>
            constexpr auto map_err(Func&& f) {
                using result_value_type = typename std::invoke_result<Func, value_type>::type::value_type;
                return is_ok()? result<result_value_type, E>{unwrap()}
                    : result<result_value_type, E>{f(this->geterr())};
            }

            template <typename Func>
            constexpr auto or_else(Func&& f) {
                return is_ok()? *this
                    : result<T, E>{f(this->geterr())};
            }
    };

    template <typename E>
    struct result<void, E> : detail::result_storage_adds<void, E>
    {
        public:
            using value_type = void;
            using unexpected_type = error_type<E>;

            constexpr result() noexcept = default;
            constexpr result(const result&) noexcept = default;
            constexpr result(result&&) noexcept = default;
            constexpr result& operator=(const result&) noexcept = default;
            constexpr result& operator=(result&&) noexcept = default;

            constexpr result(success_type<void>&&) noexcept { this->contains_value = true; }

            template <typename U> requires (std::is_convertible<U&&, E>::value)
            constexpr result(error_type<U>&& v) noexcept(std::is_nothrow_convertible<U&&, E>::value) {
                this->construct_error(static_cast<E>(forward<error_type<U>>(v)));
            }

            constexpr bool is_ok() const noexcept { return this->has_value(); }
            constexpr bool is_err() const noexcept { return !this->has_value(); }

            constexpr bool contains_err(const E&& e) const noexcept { return is_err()? e == static_cast<E>(this->geterr()) : false; }

            constexpr const E& err_value() const & {
                if (is_ok()) THROW_OR_PANIC(bad_result_access()); else return this->geterr();
            }
            constexpr E& err_value() & {
                if (is_ok()) THROW_OR_PANIC(bad_result_access()); else return this->geterr();
            }
            constexpr const E&& err_value() const && {
                if (is_ok()) THROW_OR_PANIC(bad_result_access()); else return this->geterr();
            }
            constexpr E&& err_value() && {
                if (is_ok()) THROW_OR_PANIC(bad_result_access()); else return this->geterr();
            }

            template <typename Func>
            constexpr auto and_then(Func&& f) {
                return is_err()? *this : f(valid_void_type{});
            }
    };

    #if defined (ROC_ENABLE_STD_STREAMS)
    template <typename T, typename E>
    std::ostream& operator<<(std::ostream& stream, const result<T, E>& res) {
        if (res.is_ok()) {
            if constexpr (std::is_void<T>::value)
                stream << "Ok()";
            else
                stream << "Ok(" << res.unwrap() << ")";
        } else {
            stream << "Err(" << res.err_value() << ")";
        }
        return stream;
    }
    #endif
}

namespace roc::import
{
    template <typename T> inline constexpr auto Ok(T&& t)
        noexcept(std::is_nothrow_constructible<success_type<T>, decltype(t)>::value)
    {
        if constexpr (std::is_reference<T>::value) {
            return success_type<T, true>{::roc::forward<T>(t)};
        } else {
            return success_type<T, false>{::roc::forward<T>(t)};
        }
    }
    inline constexpr success_type<void> Ok()
    {
        return success_type<void>{};
    }
    template <typename E> inline constexpr error_type<E> Err(E&& e)
        noexcept(std::is_nothrow_constructible<error_type<E>, decltype(e)>::value) {
        return error_type(forward<E>(e));
    }
}

namespace roc
{
    template <>
    struct monad_wrap<result>
    {
        template <typename T, typename E>
        constexpr static result<T, E> wrap(T&& value) {
            return result<T, E>(import::Ok(forward<T>(value)));
        }
    };

    template <>
    struct monad_bind<result>
    {
        template <typename T,
                  typename E,
                  roc::callable Func,
                  typename R = typename std::invoke_result<Func, T>::type>
        constexpr static R bind(const result<T, E>& res, Func&& f)
        {
            return res.and_then(f);
        }
    };
}

#endif
