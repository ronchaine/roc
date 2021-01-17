#include <iostream>
#include "doctest.h"

#include <array>

#define ROC_ENABLE_EXCEPTIONS

#include <roc/result.hpp>

TEST_CASE("result - correct exceptions are thrown") {
    using roc::import::Ok;
    using roc::import::Err;

    roc::result<int, int> i = Err(2);
    REQUIRE_THROWS_AS(i.unwrap(), roc::bad_result_access);
    REQUIRE(i.contains_err(2));

    i = Ok(42); // just some valid value
    REQUIRE(i.unwrap());
}
