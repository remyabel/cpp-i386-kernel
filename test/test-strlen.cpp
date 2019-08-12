#include <catch2/catch.hpp>

#include <string.hpp>

TEST_CASE("strlen returns correct values", "[string.strlen]") {
    REQUIRE(kstd::strlen("hello") == 5);
    REQUIRE(kstd::strlen("test\0null") == 4);
    REQUIRE(kstd::strlen("") == 0);
}
