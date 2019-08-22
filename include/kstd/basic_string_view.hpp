#ifndef BASIC_STRING_VIEW_HPP
#define BASIC_STRING_VIEW_HPP

#include <kstd/char_traits.hpp>
#include <stddef.h>

namespace kstd {
inline namespace v1 {

template <class charT, class traits = char_traits<charT>>
class basic_string_view {
  public:
    using traits_type = traits;
    using value_type = charT;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using const_reference = const value_type &;

    using const_iterator = const_pointer;
    using iterator = const_iterator;

    using size_type = size_t;
    using difference_type = ptrdiff_t;

    static constexpr size_type npos = size_type(-1);

    // [string.view.cons]
    constexpr basic_string_view() noexcept : data_(nullptr) {}

    constexpr basic_string_view(const basic_string_view &) noexcept = default;
    constexpr basic_string_view &
    operator=(const basic_string_view &) noexcept = default;

    // NOLINTNEXTLINE
    constexpr basic_string_view(const charT *str)
        : size_(traits_type::length(str)), data_(str) {}
    constexpr basic_string_view(const charT *str, size_type len)
        : size_(len), data_(str) {}

    // [string.view.iterators]

    [[nodiscard]] constexpr const_iterator begin() const noexcept {
        return data_;
    }

    [[nodiscard]] constexpr const_iterator cbegin() const noexcept {
        return begin();
    }

    [[nodiscard]] constexpr const_iterator end() const noexcept {
        return begin() + size();
    }

    [[nodiscard]] constexpr const_iterator cend() const noexcept {
        return end();
    }

    // [string.view.capacity]
    [[nodiscard]] constexpr size_type size() const noexcept { return size_; }

    [[nodiscard]] constexpr size_type length() const noexcept { return size_; }

    [[nodiscard]] constexpr bool empty() const noexcept { return size_ == 0; }

    // [string.view.access]
    constexpr const_reference operator[](size_type pos) const {
        return data_[pos];
    }

    [[nodiscard]] constexpr const_reference at(size_type pos) const {
        // TODO(tom): throw exception

        return data_[pos];
    }

    [[nodiscard]] constexpr const_reference front() const { return data_[0]; }

    [[nodiscard]] constexpr const_reference back() const {
        return data_[size() - 1];
    }

    [[nodiscard]] constexpr const_pointer data() const noexcept {
        return data_;
    }

  private:
    size_type size_{0};
    const_pointer data_;
};

using string_view = basic_string_view<char>;

} // namespace v1
} // namespace kstd

#endif
