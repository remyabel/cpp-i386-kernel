#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

// [alg.copy]
template <class InputIterator, class OutputIterator>
OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result)
{
    for (auto n = 0u; n < (last - first); ++n) {
        *(result + n) = *(first + n);
    }
    return result + (last - first);
}

#endif
