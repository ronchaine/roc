#include <iostream>
#include "doctest.h"

#include <roc/option.hpp>

TEST_CASE("Triviality") {
    REQUIRE(std::is_trivially_copy_constructible<roc::option<int>>::value);
    REQUIRE(std::is_trivially_copy_assignable<roc::option<int>>::value);
    REQUIRE(std::is_trivially_move_constructible<roc::option<int>>::value);
    REQUIRE(std::is_trivially_move_assignable<roc::option<int>>::value);
    REQUIRE(std::is_trivially_destructible<roc::option<int>>::value);

    {
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

    {
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

    {
        struct T {
            T(const T&) = default;
            T(T&&) = default;
            T& operator=(const T&) = default;
            T& operator=(T&&) = default;
            ~T() = default;
        };

        REQUIRE(std::is_trivially_copy_constructible<roc::option<T&>>::value);
        REQUIRE(not std::is_trivially_copy_assignable<roc::option<T&>>::value);
        REQUIRE(std::is_trivially_move_constructible<roc::option<T&>>::value);
        REQUIRE(not std::is_trivially_move_assignable<roc::option<T&>>::value);
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
        REQUIRE(not std::is_trivially_copy_assignable<roc::option<T&>>::value);
        REQUIRE(std::is_trivially_move_constructible<roc::option<T&>>::value);
        REQUIRE(not std::is_trivially_move_assignable<roc::option<T&>>::value);
        REQUIRE(std::is_trivially_destructible<roc::option<T&>>::value);
    }
}

TEST_CASE("Assignment") {
    using roc::import::Some;

    REQUIRE(std::is_constructible<roc::option<int>, decltype(Some(12))>::value);
    REQUIRE(std::is_assignable<roc::option<int>, decltype(Some(12))>::value);
    REQUIRE(not std::is_assignable<roc::option<int>, decltype(12)>::value);

    {
        struct T
        {
            roc::option<int> value_member;
        };

        REQUIRE(std::is_trivially_copy_constructible<T>::value);
        REQUIRE(std::is_trivially_copy_assignable<T>::value);
        REQUIRE(std::is_trivially_move_constructible<T>::value);
        REQUIRE(std::is_trivially_move_assignable<T>::value);
        REQUIRE(std::is_trivially_destructible<T>::value);
    }

    {
        struct T
        {
            roc::option<int&> reference_member;
        };

        REQUIRE(std::is_trivially_copy_constructible<T>::value);
        REQUIRE(not std::is_copy_assignable<T>::value);
        REQUIRE(std::is_trivially_move_constructible<T>::value);
        REQUIRE(not std::is_move_assignable<T>::value);
        REQUIRE(std::is_trivially_destructible<T>::value);
    }
}

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
