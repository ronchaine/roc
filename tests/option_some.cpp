#include <iostream>
#include "doctest.h"

#include <array>

#include <roc/option.hpp>

TEST_CASE("option constructible from Some(T) / None") {
    using roc::import::Some;
    using roc::import::None;

    SUBCASE("builtin type") {
        REQUIRE(std::is_constructible<roc::option<int>, decltype(Some(20))>::value);
    }

    SUBCASE("None") {
        REQUIRE(std::is_constructible<roc::option<int>, decltype(None)>::value);
    }

    SUBCASE("trivial type") {
        struct T {
            T() = default;
            T(const T&) = default;
            T(T&&) = default;
            T& operator=(const T&) = default;
            T& operator=(T&&) = default;
            ~T() = default;
        };

        REQUIRE(std::is_constructible<roc::option<T>, decltype(Some(T{}))>::value);
    }

    SUBCASE("nontrivial type") {
        struct T {
            T() {}
            T(const T&) {}
            T(T&&) {}
            T& operator=(const T&) { return *this; }
            T& operator=(T&&) { return *this; }
            ~T() {}
        };

        REQUIRE(std::is_constructible<roc::option<T>, decltype(Some(T{}))>::value);
    }

    SUBCASE("void type") {
        REQUIRE(std::is_constructible<roc::option<void>, decltype(Some())>::value);
    }

    SUBCASE("reference type") {
        struct T {};
        T direct;
        T& ref = direct;
        REQUIRE(std::is_constructible<roc::option<int&>, decltype(Some(ref))>::value);
    }
}

TEST_CASE("Option gets valid values from Some()") {
    using roc::import::Some;

    roc::option<int> a = Some(52),
                     b = Some(57),
                     c = Some(28);
    REQUIRE(a.contains(52));
    REQUIRE(b.contains(57));
    REQUIRE(c.contains(28));
    REQUIRE(a.is_some());
    REQUIRE(b.is_some());
    REQUIRE(c.is_some());
    REQUIRE(not a.is_none());
    REQUIRE(not b.is_none());
    REQUIRE(not c.is_none());
}

TEST_CASE("Option gets valid values from None") {
    using roc::import::None;
}
