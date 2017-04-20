#include "catch.hpp"

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
        
        string_view s2("long string", 4);

        REQUIRE(s2.length() == 4);
    }

}
