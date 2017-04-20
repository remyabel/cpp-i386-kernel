#include "catch.hpp"

#include <Terminal.hpp>

TEST_CASE("Terminal color helper functions produce correct values", "[terminal.color]") {

    SECTION("make color") {
        REQUIRE(Terminal::make_color(color::white, color::black) == static_cast<color>(15 | (0 << 4)));
        REQUIRE(Terminal::make_color(color::blue, color::red) == static_cast<color>(1 | (4 << 4)));
    }

    SECTION("make colored char") {
        REQUIRE(Terminal::make_colored_char('a', color::white)
                == ('a' | static_cast<uint16_t>(color::white) << 8));
    }
}
