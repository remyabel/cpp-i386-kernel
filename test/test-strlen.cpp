#include <catch2/catch.hpp>

#include <string.hpp>

TEST_CASE("strlen returns correct values", "[string.strlen]") {
    using namespace kstd;

    REQUIRE(strlen("hello") == 5);
    REQUIRE(strlen("test\0null") == 4);
    REQUIRE(strlen("") == 0);
}
