#include <catch2/catch.hpp>
#include <kstd/basic_string_view.hpp>
#include <kstd/iterator.hpp>

TEST_CASE("basic_string_view constructors", "[basic_string_view]") {
    SECTION("default construct") {
        kstd::string_view s;

        REQUIRE(s.length() == 0);
        REQUIRE(s.data() == nullptr);
    }

    SECTION("takes const char*") {
        kstd::string_view s("hello");

        REQUIRE(s.length() == 5);
        REQUIRE(kstd::char_traits<char>::compare("hello", s.data(), 5) == 0);

        kstd::string_view s2("long string", 4);

        REQUIRE(s2.length() == 4);
        REQUIRE(kstd::char_traits<char>::compare("long", s2.data(), 4) == 0);
    }
}

TEST_CASE("basic_string_view interface", "[basic_string_view.interface]") {
    kstd::string_view s{"hello"};
    kstd::string_view s2{"long string", 4};

    SECTION("iterators") {
        REQUIRE(s.begin() == s.cbegin());
        REQUIRE(*s.begin() == 'h');
        REQUIRE(kstd::next(s.begin(), 5) == s.end());
        REQUIRE(s.size() == s.length());

        REQUIRE(s2.size() == 4);
        REQUIRE(s2.size() == (s.size() - 1));

        REQUIRE(s.begin() != s2.begin());
    }

    kstd::string_view empty;

    SECTION("capacity") {
        REQUIRE(!s.empty());
        REQUIRE(!s2.empty());

        REQUIRE(empty.empty());
        REQUIRE(empty.size() == 0);
    }

    SECTION("access") {
        REQUIRE(s[0] == 'h');
        REQUIRE(s2[0] == 'l');

        REQUIRE(s.at(0) == s[0]);
        REQUIRE(s2.at(1) == 'o');

        REQUIRE(s[0] == s.front());
        REQUIRE(s2.back() == 'g');
    }
}
