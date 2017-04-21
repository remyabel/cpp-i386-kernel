#include "catch.hpp"

#include <iterator.hpp>
#include <basic_string_view.hpp>

TEST_CASE("basic_string_view constructors", "[basic_string_view]") {

    SECTION("default construct") {
        string_view s;

        REQUIRE(s.length() == 0);
        REQUIRE(s.data() == nullptr);
    }

    SECTION("takes const char*") {
        string_view s("hello");

        REQUIRE(s.length() == 5);
        REQUIRE(char_traits<char>::compare("hello", s.data(), 5) == 0);
        
        string_view s2("long string", 4);

        REQUIRE(s2.length() == 4);
        REQUIRE(char_traits<char>::compare("long", s2.data(), 4) == 0);
    }

}

TEST_CASE("basic_string_view interface", "[basic_string_view.interface]") {
    string_view s{"hello"};
    string_view s2{"long string", 4};

    SECTION("iterators") {
        REQUIRE(s.begin() == s.cbegin());
        REQUIRE(*s.begin() == 'h');
        REQUIRE(next(s.begin(), 5) == s.end());
        REQUIRE(s.size() == s.length());

        REQUIRE(s2.size() == 4);
        REQUIRE(s2.size() == (s.size() - 1));

        REQUIRE(s.begin() != s2.begin());
    }

    string_view empty;

    SECTION("capacity") {
        REQUIRE(!s.empty());
        REQUIRE(!s2.empty());

        REQUIRE(empty.empty());
        REQUIRE(empty.size() == 0);
    }

    SECTION("access") {
        REQUIRE(s[0] == 'h');
        REQUIRE(s2[0] == 'l');
    }
}
