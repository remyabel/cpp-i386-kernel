#include <catch2/catch.hpp>

#include <Terminal.hpp>

TEST_CASE("Terminal color helper functions produce correct values",
          "[terminal.color]") {

    using namespace vga;

    SECTION("make color") {
        REQUIRE(make_color(color::white, color::black) ==
                static_cast<color>(15 | (0 << 4)));
        REQUIRE(make_color(color::blue, color::red) ==
                static_cast<color>(1 | (4 << 4)));
    }

    SECTION("make colored char") {
        REQUIRE(make_colored_char('a', color::white) ==
                ('a' | static_cast<uint16_t>(color::white) << 8));
    }
}
