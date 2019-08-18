#ifndef STRING_H
#define STRING_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

inline size_t strlen(const char *str) {
    size_t len = 0;
    while (str[len] != 0) {
        len++;
    }
    return len;
}

inline char *strcpy(char *dest, const char *src) {
    char *temp = dest;
    while ((*dest++ = *src++) != 0) {
        ;
    }
    return temp;
}

inline int strcmp(const char *s1, const char *s2) {
    while ((*s1 != 0) && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

#ifdef __cplusplus
}
#endif

#endif
