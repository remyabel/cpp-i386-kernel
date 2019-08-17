#include <Terminal.hpp>

extern "C" void putc(void * /*unused*/, char c) { vga::terminal.put_char(c); }

namespace vga {
Terminal<char, kstd::char_traits<char>> terminal;

template <class charT, class traits>
Terminal<charT, traits>::Terminal()
    : color_(make_color(internal::color::white, internal::color::black)),
      buffer_(reinterpret_cast<uint16_t *>(0xB8000)) {
    init_printf(nullptr, putc);
    clear();
}

template <class charT, class traits>
void Terminal<charT, traits>::set_color(internal::color c) {
    color_ = c;
}

template <class charT, class traits>
void Terminal<charT, traits>::write_char_at(char c, internal::color color,
                                            size_t x, size_t y) {
    const size_t index = y * columns + x;
    buffer_[index] = static_cast<uint16_t>(make_colored_char(c, color));
}

template <class charT, class traits>
void Terminal<charT, traits>::put_char(char c) {
    if (c == '\n') {
        column_ = 0;
        row_++;
    } else {
        write_char_at(c, color_, column_, row_);
        column_++;
    }

    if (column_ == columns) {
        column_ = 0;
        ++row_;
    }

    if (row_ == rows) {
        row_ = rows - 1;

        // Copy each line upwards
        for (auto y = 0U; y < rows; y++) {
            auto next_line_start = (y + 1) * columns;
            auto next_line_end = ((y + 1) * columns) + columns;
            auto current_line_start = y * columns;

            kstd::copy(buffer_ + next_line_start, buffer_ + next_line_end,
                       buffer_ + current_line_start);
        }

        clear_line(rows - 1);
    }

    move_cursor();
}

template <class charT, class traits> void Terminal<charT, traits>::clear() {
    for (auto y = 0U; y < rows; ++y) {
        for (auto x = 0U; x < columns; ++x) {
            write_char_at(
                ' ', make_color(internal::color::white, internal::color::black),
                x, y);
        }
    }
}

template <class charT, class traits>
void Terminal<charT, traits>::clear_line(size_t row) {
    for (auto x = 0U; x < columns; ++x) {
        write_char_at(
            ' ', make_color(internal::color::white, internal::color::black), x,
            row);
    }
}

template <class charT, class traits>
void Terminal<charT, traits>::move_cursor() {
    auto position = (row_ * columns) + column_;

    outb(index_register::underline_location, index_register::high_byte);
    outb(index_register::underline_location + 1,
         static_cast<uint8_t>(position >> 8));

    outb(index_register::underline_location, index_register::low_byte);
    outb(index_register::underline_location + 1,
         static_cast<uint8_t>(position));
}
} // namespace vga
