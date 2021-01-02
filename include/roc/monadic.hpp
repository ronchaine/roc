#ifndef ROC_MONADIC_HPP
#define ROC_MONADIC_HPP

#include "utility.hpp"

namespace roc
{
    template <template <typename...> typename Monad>
    struct monad_wrap
    {
        template <typename T>
        constexpr static Monad<T> wrap(T&& value) {
            return Monad<T>(forward<T>(value));
        }
    };

    template <template <typename...> typename Monad>
    struct monad_bind {
        static_assert(dependent_false<Monad<int>>(), "cannot use nonspecialized bind");
    };

    template <typename T>
    concept callable = requires(T a) {
        { std::is_invocable<T>::value };
    };

    template <template <typename...> typename Monad, typename T>
    concept monad = requires(Monad<T> a) {
        monad_wrap<Monad>::wrap(std::declval<T>());
        monad_bind<Monad>::bind; // improve this somehow?
    };
}

#endif
