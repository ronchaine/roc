#include <iostream>
#include "doctest.h"

#include <array>

#include "../option.hpp"

TEST_CASE("option constructible from Some(T)") {
    using roc::import::Some;
    SUBCASE("builtin type") {
        REQUIRE(std::is_constructible<roc::option<int>, decltype(Some(20))>::value);
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

    std::array<roc::option<int>, 15> numbers;
    for (int i = 0; i < 15; ++i)
        numbers[i] = Some(i);

//    REQUIRE()
}

