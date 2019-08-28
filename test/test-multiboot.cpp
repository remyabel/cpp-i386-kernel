#include <catch2/catch.hpp>
#include <kstd/array.hpp>
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
    multiboot_tag_mmap fake_mmap_tag{MULTIBOOT_TAG_TYPE_MMAP,
                                     sizeof(multiboot_tag_mmap) +
                                         sizeof(fake_mmap_entries),
                                     sizeof(multiboot_mmap_entry), 0};
    Multiboot_mmap_iterator test{&fake_mmap_tag};
    Multiboot_mmap_iterator sentinel{
        &fake_mmap_tag + fake_mmap_tag.size / sizeof(multiboot_mmap_entry)};
    REQUIRE(test != sentinel);
    REQUIRE(*test == fake_mmap_entries[0]);
    ++test;
    REQUIRE(test != sentinel);
    REQUIRE(*test == fake_mmap_entries[1]);
    ++test;
}
