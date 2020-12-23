#include <iostream>
#include "doctest.h"

#include <array>

#define ROC_ENABLE_EXCEPTIONS

#include <roc/option.hpp>

TEST_CASE("Correct exceptions are thrown") {
    using roc::import::Some;
    using roc::import::None;

    roc::option<int> i = None;
    REQUIRE_THROWS_AS(i.unwrap(), roc::bad_option_access);
    
    i = Some(42); // just some valid value
    REQUIRE(i.unwrap());
}
