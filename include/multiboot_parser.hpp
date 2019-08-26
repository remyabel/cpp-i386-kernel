#ifndef MULTIBOOT_PARSER_HPP
#define MULTIBOOT_PARSER_HPP

#include <kstd/iterator.hpp>
#include <multiboot2.hpp>
#include <stddef.h>
#include <stdint.h>

class Multiboot_tag_iterator {
  public:
    using difference_type = ptrdiff_t;
    using value_type = multiboot_tag;
    using pointer = value_type *;
    using reference = value_type &;
    using iterator_category = kstd::forward_iterator_tag;

    explicit Multiboot_tag_iterator(uint32_t multiboot_addr)
        : addr_(reinterpret_cast<multiboot_tag *>(multiboot_addr)) {}

    reference operator*() const { return *addr_; }
    pointer operator->() const { return addr_; }
    Multiboot_tag_iterator &operator++() {
        next_tag();
        return *this;
    }
    Multiboot_tag_iterator operator++(int) {
        Multiboot_tag_iterator tmp = *this;
        next_tag();
        return tmp;
    }

  private:
    void next_tag() {
        addr_ = reinterpret_cast<multiboot_tag *>(
            reinterpret_cast<multiboot_uint8_t *>(addr_) +
            // Account for alignment/padding.
            ((addr_->size + 7) & ~7));
    }
    multiboot_tag *addr_;
};

#endif
