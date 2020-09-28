#include "catch.hpp"

#include "factorial.hpp"

TEST_CASE("Factorials are computed -- inside bounds", "[basic]") {
    REQUIRE(factorial(1) == 1);
    REQUIRE(factorial(2) == 2);
    REQUIRE(factorial(3) == 6);
    REQUIRE(factorial(10) == 3628800);
}

TEST_CASE("Factorials are computed -- boundary values", "[boundaries]") {
    REQUIRE(factorial(0) == 1);
    REQUIRE(factorial(11) == 39916800);
}
