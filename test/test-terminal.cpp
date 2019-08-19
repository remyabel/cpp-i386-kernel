#include <catch2/catch.hpp>
#include <terminal.hpp>

TEST_CASE("Terminal color helper functions produce correct values",
          "[terminal.color]") {

    using namespace vga;

    SECTION("make color") {
        REQUIRE(make_color(internal::color::white, internal::color::black) ==
                static_cast<internal::color>(15 | (0 << 4)));
        REQUIRE(make_color(internal::color::blue, internal::color::red) ==
                static_cast<internal::color>(1 | (4 << 4)));
    }

    SECTION("make colored char") {
        REQUIRE(static_cast<uint16_t>(
                    make_colored_char('a', internal::color::white)) ==
                ('a' | static_cast<uint16_t>(internal::color::white) << 8));
    }
}
