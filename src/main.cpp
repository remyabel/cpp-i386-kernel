#include <kstd/algorithm.hpp>
#include <kstd/array.hpp>
#include <kstd/basic_string_view.hpp>
#include <kstd/string.hpp>
#include <gdt.hpp>
#include <idt.hpp>
#include <multiboot2.hpp>
#include <multiboot_parser.hpp>
#include <paging.hpp>
#include <serial.hpp>
#include <stddef.h>
#include <stdint.h>
#include <terminal.hpp>
#include <terminal_iterator.hpp>

#ifdef ENABLE_LIBGCOV_PORT
#include <libgcov-embedded.h>
#endif

extern void global_constructor_test();
extern int local_static_variable_test();

using page_directory_entry = uint32_t;
using page_table_entry = uint32_t;

alignas(4096) kstd::array<page_directory_entry, 1024> boot_page_directory;
alignas(4096) kstd::array<page_table_entry, 1024> boot_page_table;

extern "C" void early_main() {
    auto addr = 0;
    // NOLINTNEXTLINE
    for (auto i = 0u; i < boot_page_table.size(); ++i) {
        boot_page_table[i] = addr | 3;
        addr += 4096;
    }
    boot_page_directory[0] = reinterpret_cast<uintptr_t>(&boot_page_table) | 3;

    for (auto i = 1; i < boot_page_directory.size() - 1; ++i) {
        boot_page_directory[1] = page_directory_entry{};
    }
}

// NOLINTNEXTLINE
extern "C" void kernel_main(uint32_t magic, uint32_t addr) {
    const Gdt gdt;
    const Idt idt;

#ifdef ENABLE_LIBGCOV_PORT
    static_init();
#endif

    kstd::string_view s{"Initialized GDT"};
    kstd::copy(s.begin(), s.end(),
               vga::Terminal_output_iterator<char>(vga::terminal, ""));
    printf("\n");

    vga::terminal << "Testing operator<<\n";

    printf("Initialized IDT\n");

    global_constructor_test();

    for (int i = 0; i < 5; ++i) {
        local_static_variable_test();
    }
    printf("%d\n", local_static_variable_test());

    serial.write('t');

    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
        printf("Invalid magic number. Found 0x%x but expected 0x%x\n", magic,
               MULTIBOOT2_BOOTLOADER_MAGIC);
        return;
    }
    if ((addr & 7) != 0U) {
        printf("Unaligned mbi: 0x%x\n", addr);
        return;
    }

    // Add 8 to skip over the size.
    Multiboot_tag_iterator tag_it{addr + 8};
    while (tag_it->type != MULTIBOOT_TAG_TYPE_END) {
        switch (tag_it->type) {
        case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO: {
            auto *meminfo =
                reinterpret_cast<multiboot_tag_basic_meminfo *>(&*tag_it);
            printf("mem_lower = %uKB, mem_upper = %uKB\n", meminfo->mem_lower,
                   meminfo->mem_upper);
        } break;
        case MULTIBOOT_TAG_TYPE_MMAP: {
            auto *mmap_tag = reinterpret_cast<multiboot_tag_mmap *>(&*tag_it);
            Multiboot_mmap_range mmap_range(mmap_tag);
            auto mmap_it = mmap_range.begin();
            while (mmap_it != mmap_range.end()) {
                printf(" base_addr = 0x%x%x,"
                       " length = 0x%x%x, type = %s\n",
                       static_cast<uint32_t>(mmap_it->addr >> 32),
                       static_cast<uint32_t>(mmap_it->addr & 0xffffffff),
                       static_cast<uint32_t>(mmap_it->len >> 32),
                       static_cast<uint32_t>(mmap_it->len & 0xffffffff),
                       kstd::to_string(
                           static_cast<MultibootMemoryType>(mmap_it->type)));
                ++mmap_it;
            }
        } break;
        }
        ++tag_it;
    }
    asm volatile("int $0x3");
    asm volatile("int $0x4");
}
