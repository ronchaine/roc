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

    SUBCASE("type with trivial result value member is trivially copyable/movable/destructible") {
        struct T {
            roc::result<int, int> builtins;
            roc::result<trivial_type, int> trivial_value;
            roc::result<int, trivial_type> trivial_error;
            roc::result<trivial_type, trivial_type> both_trivial;
        };

        REQUIRE(std::is_trivially_copy_constructible<T>::value);
        REQUIRE(std::is_trivially_copy_assignable<T>::value);
        REQUIRE(std::is_trivially_move_constructible<T>::value);
        REQUIRE(std::is_trivially_move_assignable<T>::value);
        REQUIRE(std::is_trivially_destructible<T>::value);
    }

    SUBCASE("type with result<T&> value member is still trivial sans assignments") {
        struct T {
            roc::result<int&, int> builtins;
            roc::result<trivial_type&, int> trivial_value;
        };

        REQUIRE(std::is_trivially_copy_constructible<T>::value);
        REQUIRE(std::is_trivially_move_constructible<T>::value);
        REQUIRE(not std::is_trivially_copy_assignable<T>::value);
        REQUIRE(not std::is_trivially_move_assignable<T>::value);
        REQUIRE(std::is_trivially_destructible<T>::value);
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
    SUBCASE("default construct") {
        roc::result<int, int> value;
        roc::result<int&, int> reference;
        roc::result<void, int> void_;

        REQUIRE(not value.is_ok());
        REQUIRE(value.is_err());
        REQUIRE(not reference.is_ok());
        REQUIRE(reference.is_err());
        REQUIRE(not void_.is_ok());
        REQUIRE(void_.is_err());
    }
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

        REQUIRE(ref71.contains(71));
        REQUIRE(ref71.unwrap() == 71);
    }
    SUBCASE("initialise and verify (void)") {
        roc::result<void, int> void_test { Ok() };

        REQUIRE(void_test.is_ok());
        REQUIRE(not void_test.is_err());
    }
}

TEST_CASE("roc::result - assigning / checking values") {
    SUBCASE("assign and verify (value)") {
        roc::result<int, int> test_case;
        REQUIRE(not test_case.is_ok());

        test_case = Ok(22);
        REQUIRE(test_case.is_ok());
        REQUIRE(test_case.contains(22));
        REQUIRE(test_case.unwrap() == 22);

        test_case = Err(42);
        REQUIRE(test_case.is_err());
        REQUIRE(test_case.contains_err(42));
    }

    SUBCASE("assign and verify (reference)") {
        roc::result<int&, int> test_case;
        REQUIRE(not test_case.is_ok());

        test_case = Err(42);
        REQUIRE(test_case.is_err());
        REQUIRE(test_case.contains_err(42));
    }

    SUBCASE("assign and verify (void)") {
        roc::result<void, int> test_case;
        REQUIRE(not test_case.is_ok());

        test_case = Err(42);
        REQUIRE(test_case.is_err());
        REQUIRE(test_case.contains_err(42));

        test_case = Ok();
        REQUIRE(test_case.is_ok());
    }
}
