#include <catch2/catch.hpp>

#include <string.hpp>

TEST_CASE("strlen returns correct values", "[string.strlen]") {
    REQUIRE(kstd::strlen("hello") == 5);
    REQUIRE(kstd::strlen("test\0null") == 4);
    REQUIRE(kstd::strlen("") == 0);
}

TEST_CASE("strcpy copies correctly", "[string.strcpy]") {
    const char *src = "hello";
    char dest[6];
    kstd::strcpy(dest, src);
    REQUIRE(kstd::strlen(dest) == 5);
    REQUIRE(kstd::strcmp(src, dest) == 0);
}

TEST_CASE("strcmp compares correctly", "[string.strcmp]") {
    const char *src = "hello";
    const char *dest = "hello";
    REQUIRE(kstd::strcmp(src, dest) == 0);
}
