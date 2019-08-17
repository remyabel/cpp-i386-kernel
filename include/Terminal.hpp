#ifndef TERMINAL_HPP
#define TERMINAL_HPP

#include <stddef.h>
#include <stdint.h>

#include <inline_asm.hpp>

#include <algorithm.hpp>
#include <basic_string_view.hpp>
#include <string.hpp>

#include <tinyprintf.h>

namespace vga {
struct index_register {
    static constexpr auto low_byte = 15u;
    static constexpr auto high_byte = 14u;
    static constexpr auto underline_location = 0x3D4u;
};

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

    operator uint16_t() const {
        return (code_byte | static_cast<uint16_t>(attribute) << 8);
    }
};

inline namespace {
color make_color(color fg, color bg) {
    return static_cast<color>(static_cast<uint8_t>(fg) |
                              (static_cast<uint8_t>(bg) << 4));
}

colored_char make_colored_char(char code_byte, color attribute) {
    return {code_byte, attribute};
}
} // namespace

template <class charT, class traits = kstd::char_traits<charT>>
class Terminal {
    size_t row_;
    size_t column_;
    color color_;
    uint16_t *buffer_;

  public:
    Terminal();

    static constexpr size_t columns = 80;
    static constexpr size_t rows = 25;

    void set_color(color c);

    void write_char_at(char c, color color, size_t x, size_t y);

    void put_char(char c);

    void clear();

    void clear_line(size_t row);

    void move_cursor();
};

template<class charT, class traits>
inline Terminal<charT, traits>& operator<<(Terminal<charT, traits>& t, charT c) {
    printf("%c", c);
    return t;
}

template<class charT, class traits>
inline Terminal<charT, traits>& operator<<(Terminal<charT, traits>& t, const charT* s) {
    printf("%s", s);
    return t;
}

extern Terminal<char, kstd::char_traits<char>> terminal;
} // namespace vga

#endif
