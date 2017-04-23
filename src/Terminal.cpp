#include <Terminal.hpp>

namespace vga {
    Terminal terminal;

    Terminal::Terminal() 
        : row_(0), column_(0),
        color_(make_color(color::white, color::black)),
        buffer_(reinterpret_cast<uint16_t*>(0xB8000))
    { 
        clear();
    }

    void Terminal::set_color(color c) {
        color_ = c;
    }

    void Terminal::write_char_at(char c, color color, size_t x, size_t y) {
        const size_t index = y * columns + x;
        buffer_[index] = make_colored_char(c, color);
    }

    void Terminal::put_char(char c) {
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
            for (auto y = 0u; y < rows; y++) {
                auto next_line_start    = (y + 1) * columns;
                auto next_line_end      = ((y + 1) * columns) + columns;
                auto current_line_start = y * columns;

                copy(buffer_ + next_line_start,
                        buffer_ + next_line_end,
                        buffer_ + current_line_start);
            }

            clear_line(rows - 1);


        }

        move_cursor();
    }

    void Terminal::write(string_view data) {
        for (size_t i = 0; i < data.length(); i++) {
            put_char(data[i]);
        }
    }

    void Terminal::clear() {
        for (auto y = 0u; y < rows; ++y) {
            for (auto x = 0u; x < columns; ++x) {
                write_char_at(' ', make_color(color::white, color::black), x, y);
            }
        }
    }

    void Terminal::clear_line(size_t row) {
        for (auto x = 0u; x < columns; ++x) {
            write_char_at(' ', make_color(color::white, color::black), x, row);
        }
    }

    void Terminal::move_cursor() {
        auto position = (row_ * columns) + column_;

        outb(index_register::underline_location, index_register::high_byte);
        outb(index_register::underline_location + 1, static_cast<uint8_t>(position >> 8));

        outb(index_register::underline_location, index_register::low_byte);
        outb(index_register::underline_location + 1, static_cast<uint8_t>(position));
    }
}
