#ifndef STRING_HPP
#define STRING_HPP

#include <stddef.h>

namespace kstd {
inline namespace v1 {

inline size_t strlen(const char *str) {
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

} // namespace v1
} // namespace kstd

#endif
