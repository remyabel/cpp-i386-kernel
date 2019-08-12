#include <catch2/catch.hpp>

#include <char_traits.hpp>

TEST_CASE("char_traits has correct behavior for char", "[char_traits]") {
    REQUIRE(kstd::char_traits<char>::eq('a', 'a'));
    REQUIRE(!kstd::char_traits<char>::eq('b', 'a'));

    REQUIRE(kstd::char_traits<char>::lt('a', 'b'));
    REQUIRE(!kstd::char_traits<char>::lt('a', 'a'));
    REQUIRE(!kstd::char_traits<char>::lt('b', 'a'));

    REQUIRE(kstd::char_traits<char>::length("") == 0);
    REQUIRE(kstd::char_traits<char>::length("hello") == 5);
    REQUIRE(kstd::char_traits<char>::length("test\0yeah") == 4);

    SECTION("compare") {
        REQUIRE(kstd::char_traits<char>::compare("asdf", "asdf", 4) == 0);
        REQUIRE(kstd::char_traits<char>::compare("a", "b", 1) < 0);
        REQUIRE(kstd::char_traits<char>::compare("c", "b", 1) > 0);
        REQUIRE(kstd::char_traits<char>::compare("ab", "bc", 2) < 0);
    }
}
