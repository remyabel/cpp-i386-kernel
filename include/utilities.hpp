#ifndef UTILITIES_HPP
#define UTILITIES_HPP

// [specialized.addressof]

template <class T>
constexpr T* addressof(T& r) noexcept {
    return reinterpret_cast<T*>(
            &const_cast<char&>(
                reinterpret_cast<const volatile char&>(r)
            )
        );
}

#endif
