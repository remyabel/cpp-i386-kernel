#ifndef TERMINAL_ITERATOR_HPP
#define TERMINAL_ITERATOR_HPP

#include <char_traits.hpp>
#include <iterator.hpp>
#include <stdint.h>
#include <utilities.hpp>

#include <Terminal.hpp>

namespace vga {
template <class charT = char, class traits = kstd::char_traits<charT>>
class Terminal_output_iterator {
  public:
    using iterator_category = kstd::output_iterator_tag;
    using value_type = void;
    using difference_type = void;
    using pointer = void;
    using reference = void;
    using char_type = charT;
    using traits_type = traits;
    using terminal_type = Terminal;

    Terminal_output_iterator(terminal_type &t)
        : out_terminal(kstd::addressof(t)), delim(nullptr) {}
    Terminal_output_iterator(terminal_type &t, const charT *delimiter)
        : out_terminal(kstd::addressof(t)), delim(delimiter) {}

    // Destructor/copy constructor are implicitly generated

    Terminal_output_iterator &operator=(char value) {
        printf("%c", value);
        if (delim) {
            printf(delim);
        }
        return *this;
    }

    Terminal_output_iterator &operator*() { return *this; }
    Terminal_output_iterator &operator++() { return *this; }
    Terminal_output_iterator &operator++(int) { return *this; }

  private:
    Terminal *out_terminal;
    const charT *delim;
};
} // namespace vga

#endif
