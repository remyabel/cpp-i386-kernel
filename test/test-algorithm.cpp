#include "catch.hpp"

#include <algorithm.hpp>

TEST_CASE("perform copy", "[copy]") {
    int input[] = {1, 2, 3};
    int output[3] = {};

    copy(input, input + 3, output);

    REQUIRE(output[0] == 1);
    REQUIRE(output[1] == 2);
    REQUIRE(output[2] == 3);
}
