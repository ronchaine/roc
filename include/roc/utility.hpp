#ifndef ROC_UTILITY_HPP
#define ROC_UTILITY_HPP

#include <cstdlib> // for abort
#include <type_traits>

#if defined (ROC_ENABLE_EXCEPTIONS)
# define THROW_OR_PANIC(x) throw(x)
#else
# define THROW_OR_PANIC(x) std::abort()
#endif

namespace roc::tags
{
    struct prevent_init {};
    struct in_place {};
    struct unexpected {};
}

namespace roc
{
    // move and forward, since utility isn't guaranteed in freestanding
    template <typename T> inline constexpr std::remove_reference_t<T>&& move(T&& t) noexcept {
        return static_cast<typename std::remove_reference<T>::type&&>(t);
    }
    template <typename T> inline constexpr T&& forward(typename std::remove_reference_t<T>& t) noexcept {
        return static_cast<T&&>(t);
    }
    template <typename T> inline constexpr T&& forward(typename std::remove_reference_t<T>&& t) noexcept {
        static_assert(not std::is_lvalue_reference<T>::value,
                      "Cannot forward rvalue as an lvalue.");
        return static_cast<T&&>(t);
    }

    template<class T> struct dependent_false : std::false_type {};

    struct none_type {};
    struct valid_void_type {};
    struct empty_type {};
}

namespace roc::detail
{
    // just for clarity in specialisations
    constexpr static bool IS_REFERENCE = true;
    constexpr static bool NOT_REFERENCE = false;

    constexpr static bool TRIVIALLY_DESTRUCTIBLE_VALUE = true;
    constexpr static bool NOT_TRIVIALLY_DESTRUCTIBLE_VALUE = false;

    constexpr static bool TRIVIALLY_DESTRUCTIBLE_ERROR = true;
    constexpr static bool NOT_TRIVIALLY_DESTRUCTIBLE_ERROR = false;
}

#endif
