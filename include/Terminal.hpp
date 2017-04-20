#ifndef TERMINAL_HPP
#define TERMINAL_HPP

#include <stddef.h>
#include <stdint.h>

#include <string.hpp>
#include <basic_string_view.hpp>

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
        return (code_byte |
            static_cast<uint16_t>(attribute) << 8);
    }
};
 
class Terminal {
    size_t row_;
    size_t column_;
    color color_;
    uint16_t *buffer_;
public:
    Terminal() 
        : row_(0), column_(0),
          color_(make_color(color::white, color::black)),
          buffer_(reinterpret_cast<uint16_t*>(0xB8000))
    { 
        for (auto y = 0u; y < rows; ++y) {
            for (auto x = 0u; x < columns; ++x) {
                write_char_at(' ', color::black, x, y);
            }
        }
    }

    static constexpr size_t columns = 80;
    static constexpr size_t rows = 25;

    static color make_color(color fg, color bg) {
        return static_cast<color>(
                static_cast<uint8_t>(fg) 
                | (static_cast<uint8_t>(bg) << 4)
            );
    }
    
    static colored_char make_colored_char(char code_byte, color attribute) {
        return { code_byte, attribute };
    }

    void set_color(color c) {
        color_ = c;
    }

    void write_char_at(char c, color color, size_t x, size_t y) {
        const size_t index = y * columns + x;
        buffer_[index] = make_colored_char(c, color);
    }

    void put_char(char c) {
        write_char_at(c, color_, column_, row_);
        if (++column_ == columns) {
            column_ = 0;
            if (++row_ == rows) {
                row_ = 0;
            }
        }
    }
    
    void write(string_view data) {
        for (size_t i = 0; i < data.length(); i++) {
            put_char(data[i]);
        }
    }
};

#endif
