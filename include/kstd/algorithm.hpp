#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include <iterator.hpp>
#include <stddef.h>

namespace kstd {
inline namespace v1 {

// [alg.copy]
template <class InputIterator, class OutputIterator>
OutputIterator copy(InputIterator first, InputIterator last,
                    OutputIterator result) {
    for (; first != last;) {
        *result++ = *first++;
    }
    return result;
}

} // namespace v1
} // namespace kstd

#endif
