#include <catch2/catch.hpp>

#include <char_traits.hpp>

TEST_CASE("char_traits has correct behavior for char", "[char_traits]") {
    using namespace kstd;

    REQUIRE(char_traits<char>::eq('a', 'a'));
    REQUIRE(!char_traits<char>::eq('b', 'a'));

    REQUIRE(char_traits<char>::lt('a', 'b'));
    REQUIRE(!char_traits<char>::lt('a', 'a'));
    REQUIRE(!char_traits<char>::lt('b', 'a'));

    REQUIRE(char_traits<char>::length("") == 0);
    REQUIRE(char_traits<char>::length("hello") == 5);
    REQUIRE(char_traits<char>::length("test\0yeah") == 4);

    SECTION("compare") {
        REQUIRE(char_traits<char>::compare("asdf", "asdf", 4) == 0);
        REQUIRE(char_traits<char>::compare("a", "b", 1) < 0);
        REQUIRE(char_traits<char>::compare("c", "b", 1) > 0);
        REQUIRE(char_traits<char>::compare("ab", "bc", 2) < 0);
    }
}
