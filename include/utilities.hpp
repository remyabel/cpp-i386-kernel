#ifndef UTILITIES_HPP
#define UTILITIES_HPP

namespace kstd {
    inline namespace v1 {

        // [specialized.addressof]
        template <class T>
        constexpr T* addressof(T& r) noexcept {
            return reinterpret_cast<T*>(
                    &const_cast<char&>(
                        reinterpret_cast<const volatile char&>(r)
                    )
                );
        }

    }
}

#endif
