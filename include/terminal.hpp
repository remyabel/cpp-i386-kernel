#ifndef TERMINAL_HPP
#define TERMINAL_HPP

#include <internal/terminal_helpers.hpp>
#include <algorithm.hpp>
#include <basic_string_view.hpp>
#include <inline_asm.hpp>
#include <stddef.h>
#include <stdint.h>
#include <string.hpp>
#include <tinyprintf.h>

namespace vga {
struct index_register {
    static constexpr auto low_byte = 15U;
    static constexpr auto high_byte = 14U;
    static constexpr auto underline_location = 0x3D4U;
};

template <class charT, class traits = kstd::char_traits<charT>> class Terminal {
    size_t row_{0};
    size_t column_{0};
    internal::color color_;
    uint16_t *buffer_;

  public:
    Terminal();

    static constexpr size_t columns = 80;
    static constexpr size_t rows = 25;

    void set_color(internal::color c);

    void write_char_at(char c, internal::color color, size_t x, size_t y);

    void put_char(char c);

    void clear();

    void clear_line(size_t row);

    void move_cursor();
};

template <class charT, class traits>
inline Terminal<charT, traits> &operator<<(Terminal<charT, traits> &t,
                                           charT c) {
    printf("%c", c);
    return t;
}

template <class charT, class traits>
inline Terminal<charT, traits> &operator<<(Terminal<charT, traits> &t,
                                           const charT *s) {
    printf("%s", s);
    return t;
}

extern Terminal<char, kstd::char_traits<char>> terminal;
} // namespace vga

#endif
