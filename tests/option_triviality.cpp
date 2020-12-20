#include <iostream>
#include "doctest.h"

#include "../option.hpp"

TEST_CASE("Triviality") {
    SUBCASE("default type") {
        REQUIRE(std::is_trivially_copy_constructible<roc::option<int>>::value);
        REQUIRE(std::is_trivially_copy_assignable<roc::option<int>>::value);
        REQUIRE(std::is_trivially_move_constructible<roc::option<int>>::value);
        REQUIRE(std::is_trivially_move_assignable<roc::option<int>>::value);
        REQUIRE(std::is_trivially_destructible<roc::option<int>>::value);
    }

    SUBCASE("trivial type") {
        struct T {
            T(const T&) = default;
            T(T&&) = default;
            T& operator=(const T&) = default;
            T& operator=(T&&) = default;
            ~T() = default;
        };

        REQUIRE(std::is_trivially_copy_constructible<roc::option<T>>::value);
        REQUIRE(std::is_trivially_copy_assignable<roc::option<T>>::value);
        REQUIRE(std::is_trivially_move_constructible<roc::option<T>>::value);
        REQUIRE(std::is_trivially_move_assignable<roc::option<T>>::value);
        REQUIRE(std::is_trivially_destructible<roc::option<T>>::value);
    }

    SUBCASE("nontrivial type") {
        struct T {
            T(const T&) {};
            T(T&&) {};
            T& operator=(const T&) { return *this; };
            T& operator=(T&&) { return *this; }
            ~T() {};
        };

        REQUIRE(not std::is_trivially_copy_constructible<roc::option<T>>::value);
        REQUIRE(not std::is_trivially_copy_assignable<roc::option<T>>::value);
        REQUIRE(not std::is_trivially_move_constructible<roc::option<T>>::value);
        REQUIRE(not std::is_trivially_move_assignable<roc::option<T>>::value);
        REQUIRE(not std::is_trivially_destructible<roc::option<T>>::value);
    }

    SUBCASE("reference type") {
        {
            struct T {
                T(const T&) = default;
                T(T&&) = default;
                T& operator=(const T&) = default;
                T& operator=(T&&) = default;
                ~T() = default;
            };

            REQUIRE(std::is_trivially_copy_constructible<roc::option<T&>>::value);
            REQUIRE(std::is_trivially_copy_assignable<roc::option<T&>>::value);
            REQUIRE(std::is_trivially_move_constructible<roc::option<T&>>::value);
            REQUIRE(std::is_trivially_move_assignable<roc::option<T&>>::value);
            REQUIRE(std::is_trivially_destructible<roc::option<T&>>::value);
        }
        {
            struct T {
                T(const T&) {};
                T(T&&) {};
                T& operator=(const T&) { return *this; };
                T& operator=(T&&) { return *this; }
                ~T() {};
            };

            REQUIRE(std::is_trivially_copy_constructible<roc::option<T&>>::value);
            REQUIRE(std::is_trivially_copy_assignable<roc::option<T&>>::value);
            REQUIRE(std::is_trivially_move_constructible<roc::option<T&>>::value);
            REQUIRE(std::is_trivially_move_assignable<roc::option<T&>>::value);
            REQUIRE(std::is_trivially_destructible<roc::option<T&>>::value);
        }
    }
}
