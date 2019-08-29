#include <catch2/catch.hpp>
#include <kstd/array.hpp>
#include <cstdlib>
#include <multiboot2.hpp>
#include <multiboot_parser.hpp>

TEST_CASE("Multiboot_mmap_iterator has correct semantics",
          "[multiboot.mmap_iterator.semantics]") {
    SECTION("Test equality.") {
        Multiboot_mmap_iterator it{};
        Multiboot_mmap_iterator it2{};
        REQUIRE(it == it2);

        multiboot_tag_mmap fake_mmap_tag{};
        Multiboot_mmap_iterator it3{&fake_mmap_tag};
        Multiboot_mmap_iterator it4{&fake_mmap_tag};
        REQUIRE(it3 == it4);

        multiboot_tag_mmap fake_mmap_tag2{1, 1, 1, 1};
        Multiboot_mmap_iterator it5{&fake_mmap_tag};
        Multiboot_mmap_iterator it6{&fake_mmap_tag2};
        REQUIRE(it5 != it6);
    }
}

TEST_CASE("Multiboot_map_iterator can iterate over the memory map",
          "[multiboot.mmap_iterator.iteration]") {
    kstd::array<multiboot_mmap_entry, 2> fake_mmap_entries = {
        multiboot_mmap_entry{
            0xd3adb33f, 1,
            static_cast<uint32_t>(MultibootMemoryType::Available), 0},
        multiboot_mmap_entry{
            0x12345678, 1, static_cast<uint32_t>(MultibootMemoryType::Reserved),
            0}};

    // We use malloc to initialize the zero sized array.
    multiboot_tag_mmap *fake_mmap_tag = (multiboot_tag_mmap *)malloc(
        sizeof(multiboot_tag_mmap) +
        fake_mmap_entries.size() * sizeof(multiboot_mmap_entry));
    fake_mmap_tag->type = MULTIBOOT_TAG_TYPE_MMAP;
    fake_mmap_tag->size =
        sizeof(multiboot_tag_mmap) +
        fake_mmap_entries.size() * sizeof(multiboot_mmap_entry);
    fake_mmap_tag->entry_size = sizeof(multiboot_mmap_entry);
    fake_mmap_tag->entry_version = 0;

    for (size_t i = 0u; i < fake_mmap_entries.size(); ++i) {
        fake_mmap_tag->entries[i] = fake_mmap_entries[i];
    }

    Multiboot_mmap_iterator test{fake_mmap_tag};
    Multiboot_mmap_iterator sentinel{reinterpret_cast<multiboot_tag_mmap *>(
        reinterpret_cast<multiboot_uint8_t *>(fake_mmap_tag) +
        fake_mmap_tag->size - sizeof(multiboot_uint32_t) * 4)};
    REQUIRE(test != sentinel);
    REQUIRE(*test == fake_mmap_entries[0]);
    ++test;
    REQUIRE(test != sentinel);
    REQUIRE(*test == fake_mmap_entries[1]);
    ++test;
    REQUIRE(test == sentinel);

    Multiboot_mmap_range mmap_range{fake_mmap_tag};
    auto mmap_it = mmap_range.begin();
    REQUIRE(mmap_it != sentinel);
    REQUIRE(*mmap_it == fake_mmap_entries[0]);
    ++mmap_it;
    REQUIRE(mmap_it != sentinel);
    REQUIRE(*mmap_it == fake_mmap_entries[1]);
    ++mmap_it;
    REQUIRE(mmap_it == sentinel);
}
