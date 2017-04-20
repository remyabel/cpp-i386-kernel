#ifndef BASIC_STRING_VIEW_HPP
#define BASIC_STRING_VIEW_HPP

#include <stddef.h>

#include <char_traits.hpp>

template <class charT, class traits = char_traits<charT>>
class basic_string_view {
public:
    using traits_type   = traits;
    using value_type    = charT;
    using pointer       = value_type*;
    using const_pointer = const value_type*;
    using size_type     = size_t;

    static constexpr size_type npos = size_type(-1);

    // [string.view.cons]
    constexpr basic_string_view() noexcept;
    constexpr basic_string_view(const basic_string_view&) noexcept = default;
    constexpr basic_string_view& operator=(const basic_string_view&) noexcept = default;
    constexpr basic_string_view(const charT *str);
    constexpr basic_string_view(const charT *str, size_type len);
};

using string_view = basic_string_view<char>;

#endif
