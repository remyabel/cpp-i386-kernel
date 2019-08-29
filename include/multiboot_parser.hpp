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

class Multiboot_mmap_iterator {
  public:
    using difference_type = ptrdiff_t;
    using value_type = multiboot_mmap_entry;
    using pointer = value_type *;
    using reference = value_type &;
    using iterator_category = kstd::forward_iterator_tag;

    explicit Multiboot_mmap_iterator(multiboot_tag_mmap *mmap_tag)
        : entries_(mmap_tag->entries) {}
    Multiboot_mmap_iterator() = default;

    reference operator*() const { return *entries_; }
    pointer operator->() const { return entries_; }
    Multiboot_mmap_iterator &operator++() {
        next_entry();
        return *this;
    }
    Multiboot_mmap_iterator operator++(int) {
        Multiboot_mmap_iterator tmp = *this;
        next_entry();
        return tmp;
    }

    friend bool operator==(Multiboot_mmap_iterator lhs,
                           Multiboot_mmap_iterator rhs);
    friend bool operator!=(Multiboot_mmap_iterator lhs,
                           Multiboot_mmap_iterator rhs);

  private:
    void next_entry() {
        entries_ = reinterpret_cast<multiboot_memory_map_t *>(
            reinterpret_cast<unsigned long>(entries_) +
            sizeof(multiboot_mmap_entry));
    }
    multiboot_memory_map_t *entries_;
};

/**
 * The safest way to obtain a range over the memory map is to use this
 * class. The multiboot memory map tag contains the size of the tag, but
 * not the size of the memory map entries. Thus one needs to use the size
 * of the tag as the end of the range. However since a
 * Multiboot_mmap_iterator encapsulates the entries and not the tag
 * itself, we need to subtract the size of the other tag's fields to avoid
 * an off-by-one error. This class takes care of that calculation.
 */
class Multiboot_mmap_range {
  public:
    Multiboot_mmap_range(multiboot_tag_mmap *tag)
        : begin_(tag), end_(reinterpret_cast<multiboot_tag_mmap *>(
                           reinterpret_cast<multiboot_uint8_t *>(tag) +
                           tag->size - sizeof(multiboot_uint32_t) * 4)) {}

    Multiboot_mmap_iterator begin() const { return begin_; }

    Multiboot_mmap_iterator end() const { return end_; }

  private:
    Multiboot_mmap_iterator begin_;
    Multiboot_mmap_iterator end_;
};

bool operator==(multiboot_mmap_entry lhs, multiboot_mmap_entry rhs) {
    return lhs.addr == rhs.addr && lhs.len == rhs.len && lhs.type == rhs.type;
}

bool operator==(Multiboot_mmap_iterator lhs, Multiboot_mmap_iterator rhs) {
    return lhs.entries_ == rhs.entries_;
}

bool operator!=(Multiboot_mmap_iterator lhs, Multiboot_mmap_iterator rhs) {
    return !(lhs == rhs);
}

enum class MultibootMemoryType {
    Available = MULTIBOOT_MEMORY_AVAILABLE,
    Reserved = MULTIBOOT_MEMORY_RESERVED,
    AcpiReclaimable = MULTIBOOT_MEMORY_ACPI_RECLAIMABLE,
    Nvs = MULTIBOOT_MEMORY_NVS,
    BadRam = MULTIBOOT_MEMORY_BADRAM
};

namespace kstd {
const char *to_string(MultibootMemoryType type) {
    switch (type) {
    case MultibootMemoryType::Available:
        return "Available";
    case MultibootMemoryType::Reserved:
        return "Reserved";
    case MultibootMemoryType::AcpiReclaimable:
        return "ACPI Reclaimable";
    case MultibootMemoryType::Nvs:
        return "NVS";
    case MultibootMemoryType::BadRam:
        return "Bad RAM";
    }
    return "Unknown";
}
} // namespace kstd
#endif
