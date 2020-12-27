#include <iostream>
#include "doctest.h"

#include <roc/result.hpp>
#include "test_types.hpp"

using namespace roc::import;

TEST_CASE("roc::result - basic traits") {
    SUBCASE("constructible/assignable with builtin value and error type") {
        REQUIRE(std::is_constructible<roc::result<int, int>>::value);
        REQUIRE(std::is_copy_constructible<roc::result<int, int>>::value);
        REQUIRE(std::is_move_constructible<roc::result<int, int>>::value);
        REQUIRE(std::is_copy_assignable<roc::result<int, int>>::value);
        REQUIRE(std::is_move_assignable<roc::result<int, int>>::value);
    }
    SUBCASE("constructible/assignable with trivial value and/or error type") {
        REQUIRE(std::is_constructible<roc::result<trivial_type, int>>::value);
        REQUIRE(std::is_copy_constructible<roc::result<trivial_type, int>>::value);
        REQUIRE(std::is_move_constructible<roc::result<trivial_type, int>>::value);
        REQUIRE(std::is_copy_assignable<roc::result<trivial_type, int>>::value);
        REQUIRE(std::is_move_assignable<roc::result<trivial_type, int>>::value);

        REQUIRE(std::is_constructible<roc::result<int, trivial_type>>::value);
        REQUIRE(std::is_copy_constructible<roc::result<int, trivial_type>>::value);
        REQUIRE(std::is_move_constructible<roc::result<int, trivial_type>>::value);
        REQUIRE(std::is_copy_assignable<roc::result<int, trivial_type>>::value);
        REQUIRE(std::is_move_assignable<roc::result<int, trivial_type>>::value);
        
        REQUIRE(std::is_constructible<roc::result<trivial_type, trivial_type>>::value);
        REQUIRE(std::is_copy_constructible<roc::result<trivial_type, trivial_type>>::value);
        REQUIRE(std::is_move_constructible<roc::result<trivial_type, trivial_type>>::value);
        REQUIRE(std::is_copy_assignable<roc::result<trivial_type, trivial_type>>::value);
        REQUIRE(std::is_move_assignable<roc::result<trivial_type, trivial_type>>::value);
    }
    SUBCASE("only default-constructible with nontrivial value type") {
        REQUIRE(std::is_constructible<roc::result<nontrivial_type, int>>::value);
        REQUIRE(not std::is_copy_constructible<roc::result<nontrivial_type, int>>::value);
        REQUIRE(not std::is_move_constructible<roc::result<nontrivial_type, int>>::value);
        REQUIRE(not std::is_copy_assignable<roc::result<nontrivial_type, int>>::value);
        REQUIRE(not std::is_move_assignable<roc::result<nontrivial_type, int>>::value);

        REQUIRE(std::is_constructible<roc::result<int, nontrivial_type>>::value);
        REQUIRE(not std::is_copy_constructible<roc::result<int, nontrivial_type>>::value);
        REQUIRE(not std::is_move_constructible<roc::result<int, nontrivial_type>>::value);
        REQUIRE(not std::is_copy_assignable<roc::result<int, nontrivial_type>>::value);
        REQUIRE(not std::is_move_assignable<roc::result<int, nontrivial_type>>::value);
        
        REQUIRE(std::is_constructible<roc::result<nontrivial_type, nontrivial_type>>::value);
        REQUIRE(not std::is_copy_constructible<roc::result<nontrivial_type, nontrivial_type>>::value);
        REQUIRE(not std::is_move_constructible<roc::result<nontrivial_type, nontrivial_type>>::value);
        REQUIRE(not std::is_copy_assignable<roc::result<nontrivial_type, nontrivial_type>>::value);
        REQUIRE(not std::is_move_assignable<roc::result<nontrivial_type, nontrivial_type>>::value);
    }
    SUBCASE("only constructible, not assignable with reference types") {
        REQUIRE(std::is_constructible<roc::result<int&, int>>::value);
        REQUIRE(std::is_copy_constructible<roc::result<int&, int>>::value);
        REQUIRE(std::is_move_constructible<roc::result<int&, int>>::value);
        REQUIRE(not std::is_copy_assignable<roc::result<int&, int>>::value);
        REQUIRE(not std::is_move_assignable<roc::result<int&, int>>::value);
    }
    SUBCASE("constructible/assignable with void value type") {
        REQUIRE(std::is_constructible<roc::result<void, int>>::value);
        REQUIRE(std::is_copy_constructible<roc::result<void, int>>::value);
        REQUIRE(std::is_move_constructible<roc::result<void, int>>::value);
        REQUIRE(std::is_copy_assignable<roc::result<void, int>>::value);
        REQUIRE(std::is_move_assignable<roc::result<void, int>>::value);
    }
}

TEST_CASE("roc::result - nothrow qualifications") {
    SUBCASE("Builtin value/error types do not cause result to become throwing") {
        REQUIRE(std::is_nothrow_constructible<roc::result<int, int>>::value);
        REQUIRE(std::is_nothrow_copy_constructible<roc::result<int, int>>::value);
        REQUIRE(std::is_nothrow_move_constructible<roc::result<int, int>>::value);
        REQUIRE(std::is_nothrow_copy_assignable<roc::result<int, int>>::value);
        REQUIRE(std::is_nothrow_move_assignable<roc::result<int, int>>::value);
        REQUIRE(std::is_nothrow_destructible<roc::result<int, int>>::value);
    }

    SUBCASE("Builtin value/error types do not cause result to become throwing") {
        REQUIRE(std::is_nothrow_constructible<roc::result<trivial_type, int>>::value);
        REQUIRE(std::is_nothrow_copy_constructible<roc::result<trivial_type, int>>::value);
        REQUIRE(std::is_nothrow_copy_assignable<roc::result<trivial_type, int>>::value);
        REQUIRE(std::is_nothrow_move_constructible<roc::result<trivial_type, int>>::value);
        REQUIRE(std::is_nothrow_move_assignable<roc::result<trivial_type, int>>::value);
        REQUIRE(std::is_nothrow_destructible<roc::result<trivial_type, int>>::value);

        REQUIRE(std::is_nothrow_constructible<roc::result<int, trivial_type>>::value);
        REQUIRE(std::is_nothrow_copy_constructible<roc::result<int, trivial_type>>::value);
        REQUIRE(std::is_nothrow_copy_assignable<roc::result<int, trivial_type>>::value);
        REQUIRE(std::is_nothrow_move_constructible<roc::result<int, trivial_type>>::value);
        REQUIRE(std::is_nothrow_move_assignable<roc::result<int, trivial_type>>::value);
        REQUIRE(std::is_nothrow_destructible<roc::result<int, trivial_type>>::value);

        REQUIRE(std::is_nothrow_constructible<roc::result<trivial_type, trivial_type>>::value);
        REQUIRE(std::is_nothrow_copy_constructible<roc::result<trivial_type, trivial_type>>::value);
        REQUIRE(std::is_nothrow_copy_assignable<roc::result<trivial_type, trivial_type>>::value);
        REQUIRE(std::is_nothrow_move_constructible<roc::result<trivial_type, trivial_type>>::value);
        REQUIRE(std::is_nothrow_move_assignable<roc::result<trivial_type, trivial_type>>::value);
        REQUIRE(std::is_nothrow_destructible<roc::result<trivial_type, trivial_type>>::value);
    }

    SUBCASE("nontrivial value/error types do not cause construction to become throwing") {
        REQUIRE(std::is_nothrow_constructible<roc::result<nontrivial_type, int>>::value);
        REQUIRE(std::is_nothrow_destructible<roc::result<nontrivial_type, int>>::value);
        REQUIRE(std::is_nothrow_constructible<roc::result<int, nontrivial_type>>::value);
        REQUIRE(std::is_nothrow_destructible<roc::result<int, nontrivial_type>>::value);
        REQUIRE(std::is_nothrow_constructible<roc::result<nontrivial_type, nontrivial_type>>::value);
        REQUIRE(std::is_nothrow_destructible<roc::result<nontrivial_type, nontrivial_type>>::value);
    }
}

TEST_CASE("roc::result - triviality") {
    SUBCASE("trivial copy/move/destruct with builtin value and error types") {
        REQUIRE(std::is_trivially_copy_constructible<roc::result<int, int>>::value);
        REQUIRE(std::is_trivially_copy_assignable<roc::result<int, int>>::value);
        REQUIRE(std::is_trivially_move_constructible<roc::result<int, int>>::value);
        REQUIRE(std::is_trivially_move_assignable<roc::result<int, int>>::value);
        REQUIRE(std::is_trivially_destructible<roc::result<int, int>>::value);
    }
    SUBCASE("trivial copy/move/destruct with trivial value and/or error types") {
        REQUIRE(std::is_trivially_copy_constructible<roc::result<trivial_type, int>>::value);
        REQUIRE(std::is_trivially_move_constructible<roc::result<trivial_type, int>>::value);
        REQUIRE(std::is_trivially_copy_assignable<roc::result<trivial_type, int>>::value);
        REQUIRE(std::is_trivially_move_assignable<roc::result<trivial_type, int>>::value);
        REQUIRE(std::is_trivially_destructible<roc::result<trivial_type, int>>::value);

        REQUIRE(std::is_trivially_copy_constructible<roc::result<int, trivial_type>>::value);
        REQUIRE(std::is_trivially_copy_assignable<roc::result<int, trivial_type>>::value);
        REQUIRE(std::is_trivially_move_constructible<roc::result<int, trivial_type>>::value);
        REQUIRE(std::is_trivially_move_assignable<roc::result<int, trivial_type>>::value);
        REQUIRE(std::is_trivially_destructible<roc::result<int, trivial_type>>::value);

        REQUIRE(std::is_trivially_copy_constructible<roc::result<trivial_type, trivial_type>>::value);
        REQUIRE(std::is_trivially_move_constructible<roc::result<trivial_type, trivial_type>>::value);
        REQUIRE(std::is_trivially_copy_assignable<roc::result<trivial_type, trivial_type>>::value);
        REQUIRE(std::is_trivially_move_assignable<roc::result<trivial_type, trivial_type>>::value);
        REQUIRE(std::is_trivially_destructible<roc::result<trivial_type, trivial_type>>::value);
    }
    SUBCASE("trivial copy/move/destruct with reference types") {
        REQUIRE(std::is_trivially_copy_constructible<roc::result<int&, int>>::value);
        REQUIRE(std::is_trivially_move_constructible<roc::result<int&, int>>::value);
        REQUIRE(std::is_trivially_destructible<roc::result<int&, int>>::value);
    }
    SUBCASE("trivial copy/move/destruct with void type") {
        REQUIRE(std::is_trivially_copy_constructible<roc::result<void, int>>::value);
        REQUIRE(std::is_trivially_copy_assignable<roc::result<void, int>>::value);
        REQUIRE(std::is_trivially_move_constructible<roc::result<void, int>>::value);
        REQUIRE(std::is_trivially_move_assignable<roc::result<void, int>>::value);
        REQUIRE(std::is_trivially_destructible<roc::result<void, int>>::value);
    }
    SUBCASE("nontrivial copy/move/destruct with nontrivial type") {
        REQUIRE(not std::is_trivially_copy_constructible<roc::result<nontrivial_type, int>>::value);
        REQUIRE(not std::is_trivially_copy_assignable<roc::result<nontrivial_type, int>>::value);
        REQUIRE(not std::is_trivially_move_constructible<roc::result<nontrivial_type, int>>::value);
        REQUIRE(not std::is_trivially_move_assignable<roc::result<nontrivial_type, int>>::value);
        REQUIRE(not std::is_trivially_destructible<roc::result<nontrivial_type, int>>::value);
        
        REQUIRE(not std::is_trivially_copy_constructible<roc::result<int, nontrivial_type>>::value);
        REQUIRE(not std::is_trivially_copy_assignable<roc::result<int, nontrivial_type>>::value);
        REQUIRE(not std::is_trivially_move_constructible<roc::result<int, nontrivial_type>>::value);
        REQUIRE(not std::is_trivially_move_assignable<roc::result<int, nontrivial_type>>::value);
        REQUIRE(not std::is_trivially_destructible<roc::result<int, nontrivial_type>>::value);

        REQUIRE(not std::is_trivially_copy_constructible<roc::result<nontrivial_type, nontrivial_type>>::value);
        REQUIRE(not std::is_trivially_copy_assignable<roc::result<nontrivial_type, nontrivial_type>>::value);
        REQUIRE(not std::is_trivially_move_constructible<roc::result<nontrivial_type, nontrivial_type>>::value);
        REQUIRE(not std::is_trivially_move_assignable<roc::result<nontrivial_type, nontrivial_type>>::value);
        REQUIRE(not std::is_trivially_destructible<roc::result<nontrivial_type, nontrivial_type>>::value);
    }
}

TEST_CASE ("roc::result - construction & assignment traits") {
    REQUIRE(std::is_constructible<roc::result<int, int>, decltype(Ok(int{}))>::value);
    REQUIRE(std::is_assignable<roc::result<int, int>, decltype(Ok(int{}))>::value);

    REQUIRE(std::is_constructible<roc::result<int, int>, decltype(Err(int{}))>::value);
    REQUIRE(std::is_assignable<roc::result<int, int>, decltype(Err(int{}))>::value);

    REQUIRE(not std::is_assignable<roc::result<int, int>, decltype(int{})>::value);

    REQUIRE(std::is_constructible<roc::result<trivial_type, int>, decltype(Ok(trivial_type{}))>::value);
    REQUIRE(std::is_assignable<roc::result<trivial_type, int>, decltype(Ok(trivial_type{}))>::value);
    REQUIRE(std::is_constructible<roc::result<trivial_type, int>, decltype(Err(int{}))>::value);
    REQUIRE(std::is_assignable<roc::result<trivial_type, int>, decltype(Err(int{}))>::value);
    REQUIRE(not std::is_assignable<roc::result<trivial_type, int>, decltype(trivial_type{})>::value);
    REQUIRE(not std::is_assignable<roc::result<trivial_type, int>, decltype(int{})>::value);

    REQUIRE(std::is_constructible<roc::result<int, trivial_type>, decltype(Ok(int{}))>::value);
    REQUIRE(std::is_assignable<roc::result<int, trivial_type>, decltype(Ok(int{}))>::value);
    REQUIRE(std::is_constructible<roc::result<int, trivial_type>, decltype(Err(trivial_type{}))>::value);
    REQUIRE(std::is_assignable<roc::result<int, trivial_type>, decltype(Err(trivial_type{}))>::value);
    REQUIRE(not std::is_assignable<roc::result<trivial_type, int>, decltype(trivial_type{})>::value);
    REQUIRE(not std::is_assignable<roc::result<trivial_type, int>, decltype(int{})>::value);

    REQUIRE(std::is_constructible<roc::result<trivial_type, trivial_type>, decltype(Ok(trivial_type{}))>::value);
    REQUIRE(std::is_assignable<roc::result<trivial_type, trivial_type>, decltype(Ok(trivial_type{}))>::value);
    REQUIRE(std::is_constructible<roc::result<trivial_type, trivial_type>, decltype(Err(trivial_type{}))>::value);
    REQUIRE(std::is_assignable<roc::result<trivial_type, trivial_type>, decltype(Err(trivial_type{}))>::value);
    REQUIRE(not std::is_assignable<roc::result<trivial_type, trivial_type>, decltype(trivial_type{})>::value);
}

TEST_CASE("roc::result - constructing / checking values") {
    SUBCASE("initialise and verify (value)") {
        roc::result<int, int> int0 { Ok(0) };
        roc::result<int, int> int42 = Ok(42);

        REQUIRE(int0.is_ok());
        REQUIRE(int42.is_ok());
        REQUIRE(not int0.is_err());
        REQUIRE(not int42.is_err());

        REQUIRE(int0.contains(0));
        REQUIRE(int42.contains(42));

        REQUIRE(int0.unwrap() == 0);
        REQUIRE(int42.unwrap() == 42);
    }
    SUBCASE("initialise and verify (reference)") {
        int value = 71;
        roc::result<int&, int> ref71 { Ok(value) };
        REQUIRE(ref71.is_ok());
        REQUIRE(not ref71.is_err());

        REQUIRE(ref71.unwrap() == 71);
    }
}

