#ifndef CHAR_TRAITS_HPP
#define CHAR_TRAITS_HPP

#include <stddef.h>

template <class charT>
class char_traits;

template <>
class char_traits<char> {
public:
    using char_type = char;
    using int_type = int;

    static constexpr void assign(char_type &r, const char_type &a);
    static char_type *assign(char_type *p, size_t count, char_type a);

    static constexpr bool eq(char_type a, char_type b);

    static constexpr bool lt(char_type a, char_type b);

    static constexpr size_t length(const char_type *s);
};

#endif
