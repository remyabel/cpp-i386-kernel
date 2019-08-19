#ifndef TERMINAL_HELPERS_HPP
#define TERMINAL_HELPERS_HPP

#include <stdint.h>

namespace vga::internal {
enum class color : uint8_t {
    black = 0,
    blue = 1,
    green = 2,
    cyan = 3,
    red = 4,
    magenta = 5,
    brown = 6,
    light_grey = 7,
    dark_grey = 8,
    light_blue = 9,
    light_green = 10,
    light_cyan = 11,
    light_red = 12,
    light_magenta = 13,
    light_brown = 14,
    white = 15,
};

struct colored_char {
    char code_byte;
    color attribute;

    explicit operator uint16_t() const {
        return (code_byte | static_cast<uint16_t>(attribute) << 8);
    }
};

inline color make_color(color fg, color bg) {
    return static_cast<color>(static_cast<uint8_t>(fg) |
                              (static_cast<uint8_t>(bg) << 4));
}

inline colored_char make_colored_char(char code_byte, color attribute) {
    return {code_byte, attribute};
}
} // namespace vga::internal

#endif // TERMINAL_HELPERS_HPP
