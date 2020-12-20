#ifndef ROC_UTILITY_HPP
#define ROC_UTILITY_HPP

#include <cstdlib> // for abort
#include <type_traits>

#if defined (ROC_ENABLE_EXCEPTIONS)
# define THROW_OR_PANIC(x) throw(x)
#else
# define THROW_OR_PANIC(x) std::abort()
#endif

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
}

#endif
