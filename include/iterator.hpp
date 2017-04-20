#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <stddef.h>

// [iterator.primitives]
template <class Iterator> struct iterator_traits;
template <class T> struct iterator_traits<T*>;
template <class T> struct iterator_traits<const T*>;

struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag: public input_iterator_tag { };
struct bidirectional_iterator_tag: public forward_iterator_tag { };
struct random_access_iterator_tag: public bidirectional_iterator_tag { };

// [iterator.traits]
template <class Iterator>
struct iterator_traits {
    using difference_type   = typename Iterator::difference_type;
    using value_type        = typename Iterator::value_type;
    using pointer           = typename Iterator::pointer;
    using reference         = typename Iterator::reference;
    using iterator_category = typename Iterator::iterator_category;
};

template <class T>
struct iterator_traits<T*> {
    using difference_type   = ptrdiff_t;
    using value_type        = T;
    using pointer           = T*;
    using reference         = T&;
    using iterator_category = random_access_iterator_tag;
};

template <class T>
struct iterator_traits<const T*> {
    using difference_type   = ptrdiff_t;
    using value_type        = T;
    using pointer           = const T*;
    using reference         = const T&;
    using iterator_category = random_access_iterator_tag;
};

// [iterator.operations]
template <class InputIterator, class Distance>
constexpr void advance(InputIterator& i, Distance n) {
    return advance(i, n, typename iterator_traits<InputIterator>::iterator_category());
}

template <class InputIterator, class Distance>
constexpr void advance(InputIterator& i, Distance n, random_access_iterator_tag) {
    i += n;
}

template <class InputIterator>
constexpr typename iterator_traits<InputIterator>::difference_type
    distance(InputIterator first, InputIterator last) {
        return distance(first, last, typename iterator_traits<InputIterator>::iterator_category());
}

template <class InputIterator>
constexpr typename iterator_traits<InputIterator>::difference_type
    distance(InputIterator first, InputIterator last, random_access_iterator_tag) {
        return (last - first);
}

#endif
