#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include <stddef.h>

// [alg.copy]
template <class InputIterator, class OutputIterator>
OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result)
{
    size_t dist = last - first;
    for (auto n = 0u; n < dist; ++n) {
        *(result + n) = *(first + n);
    }
    return result + (last - first);
}

#endif
