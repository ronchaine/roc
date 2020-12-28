#include <iostream>
#include "doctest.h"

#include <roc/result.hpp>
#include "test_types.hpp"

using namespace roc::import;

TEST_CASE("roc::success_type - basic traits") {
    SUBCASE("constructible/assignable with builtin value type") {
        REQUIRE(std::is_copy_constructible<roc::success_type<int>>::value);
        REQUIRE(std::is_move_constructible<roc::success_type<int>>::value);
        REQUIRE(std::is_copy_assignable<roc::success_type<int>>::value);
        REQUIRE(std::is_move_assignable<roc::success_type<int>>::value);
    }
    SUBCASE("constructible/assignable with trivial value type") {
        REQUIRE(std::is_copy_constructible<roc::success_type<trivial_type>>::value);
        REQUIRE(std::is_move_constructible<roc::success_type<trivial_type>>::value);
        REQUIRE(std::is_copy_assignable<roc::success_type<trivial_type>>::value);
        REQUIRE(std::is_move_assignable<roc::success_type<trivial_type>>::value);
    }
    SUBCASE("constructible/assignable with nontrivial value type") {
        REQUIRE(std::is_copy_constructible<roc::success_type<nontrivial_type>>::value);
        REQUIRE(std::is_move_constructible<roc::success_type<nontrivial_type>>::value);
        REQUIRE(std::is_copy_assignable<roc::success_type<nontrivial_type>>::value);
        REQUIRE(std::is_move_assignable<roc::success_type<nontrivial_type>>::value);
    }
    SUBCASE("constructible with reference type") {
        REQUIRE(std::is_copy_constructible<roc::success_type<int&>>::value);
        REQUIRE(std::is_move_constructible<roc::success_type<int&>>::value);
    }
    SUBCASE("constructible with void type") {
        REQUIRE(std::is_copy_constructible<roc::success_type<void>>::value);
        REQUIRE(std::is_move_constructible<roc::success_type<void>>::value);
        REQUIRE(std::is_copy_assignable<roc::success_type<void>>::value);
        REQUIRE(std::is_move_assignable<roc::success_type<void>>::value);
    }
}
TEST_CASE("roc::success_type - triviality") {
    SUBCASE("trivial construct/assign/destruct with builtin value types") {
        REQUIRE(std::is_trivially_copy_constructible<roc::success_type<int>>::value);
        REQUIRE(std::is_trivially_move_constructible<roc::success_type<int>>::value);
        REQUIRE(std::is_trivially_copy_assignable<roc::success_type<int>>::value);
        REQUIRE(std::is_trivially_move_assignable<roc::success_type<int>>::value);
        REQUIRE(std::is_trivially_destructible<roc::success_type<int>>::value);
    }
    
    SUBCASE("trivial construct/assign/destruct with trivial value types") {
        REQUIRE(std::is_trivially_copy_constructible<roc::success_type<trivial_type>>::value);
        REQUIRE(std::is_trivially_move_constructible<roc::success_type<trivial_type>>::value);
        REQUIRE(std::is_trivially_copy_assignable<roc::success_type<trivial_type>>::value);
        REQUIRE(std::is_trivially_move_assignable<roc::success_type<trivial_type>>::value);
        REQUIRE(std::is_trivially_destructible<roc::success_type<trivial_type>>::value);
    }

    SUBCASE("nontrivial construct/assign/destruct with nontrivial value types") {
        REQUIRE(not std::is_trivially_copy_constructible<roc::success_type<nontrivial_type>>::value);
        REQUIRE(not std::is_trivially_move_constructible<roc::success_type<nontrivial_type>>::value);
        REQUIRE(not std::is_trivially_copy_assignable<roc::success_type<nontrivial_type>>::value);
        REQUIRE(not std::is_trivially_move_assignable<roc::success_type<nontrivial_type>>::value);
        REQUIRE(not std::is_trivially_destructible<roc::success_type<nontrivial_type>>::value);
    }

    SUBCASE("trivial construct/assign/destruct with reference types") {
        REQUIRE(std::is_trivially_copy_constructible<roc::success_type<int&>>::value);
        REQUIRE(std::is_trivially_move_constructible<roc::success_type<int&>>::value);
        REQUIRE(not std::is_copy_assignable<roc::success_type<int&>>::value);
        REQUIRE(not std::is_move_assignable<roc::success_type<int&>>::value);
        REQUIRE(std::is_trivially_destructible<roc::success_type<int&>>::value);
    }

    SUBCASE("trivial construct/assign/destruct with void type") {
        REQUIRE(std::is_trivially_copy_constructible<roc::success_type<void>>::value);
        REQUIRE(std::is_trivially_move_constructible<roc::success_type<void>>::value);
        REQUIRE(std::is_trivially_copy_assignable<roc::success_type<void>>::value);
        REQUIRE(std::is_trivially_move_assignable<roc::success_type<void>>::value);
        REQUIRE(std::is_trivially_destructible<roc::success_type<void>>::value);
    }
}
TEST_CASE("roc::success_type - nothrow qualifications") {
    SUBCASE("builtin value types do not cause success_type to become throwing") {
        REQUIRE(std::is_nothrow_copy_constructible<roc::success_type<int>>::value);
        REQUIRE(std::is_nothrow_move_constructible<roc::success_type<int>>::value);
        REQUIRE(std::is_nothrow_copy_assignable<roc::success_type<int>>::value);
        REQUIRE(std::is_nothrow_move_assignable<roc::success_type<int>>::value);
    }

    SUBCASE("trivial value types do not cause success_type to become throwing") {
        REQUIRE(std::is_nothrow_copy_constructible<roc::success_type<trivial_type>>::value);
        REQUIRE(std::is_nothrow_move_constructible<roc::success_type<trivial_type>>::value);
        REQUIRE(std::is_nothrow_copy_assignable<roc::success_type<trivial_type>>::value);
        REQUIRE(std::is_nothrow_move_assignable<roc::success_type<trivial_type>>::value);
    }
    
    SUBCASE("reference types do not cause success_type to become throwing") {
        REQUIRE(std::is_nothrow_copy_constructible<roc::success_type<int&>>::value);
        REQUIRE(std::is_nothrow_move_constructible<roc::success_type<int&>>::value);
    }

    SUBCASE("nontrivial value types cause success_type to become throwing, unless marked noexcept") {
        REQUIRE(not std::is_nothrow_copy_constructible<roc::success_type<nontrivial_type>>::value);
        REQUIRE(not std::is_nothrow_move_constructible<roc::success_type<nontrivial_type>>::value);
        REQUIRE(not std::is_nothrow_copy_assignable<roc::success_type<nontrivial_type>>::value);
        REQUIRE(not std::is_nothrow_move_assignable<roc::success_type<nontrivial_type>>::value);

        REQUIRE(std::is_nothrow_copy_constructible<roc::success_type<nontrivial_noexcept_type>>::value);
        REQUIRE(std::is_nothrow_move_constructible<roc::success_type<nontrivial_noexcept_type>>::value);
        REQUIRE(std::is_nothrow_copy_assignable<roc::success_type<nontrivial_noexcept_type>>::value);
        REQUIRE(std::is_nothrow_move_assignable<roc::success_type<nontrivial_noexcept_type>>::value);
    }

    SUBCASE("void type do not cause success_type to become throwing") {
        REQUIRE(std::is_nothrow_copy_constructible<roc::success_type<void>>::value);
        REQUIRE(std::is_nothrow_move_constructible<roc::success_type<void>>::value);
        REQUIRE(std::is_nothrow_copy_assignable<roc::success_type<void>>::value);
        REQUIRE(std::is_nothrow_move_assignable<roc::success_type<void>>::value);
    }
}
