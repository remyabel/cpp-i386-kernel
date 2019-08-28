#include <kstd/algorithm.hpp>
#include <kstd/basic_string_view.hpp>
#include <kstd/string.hpp>
#include <gdt.hpp>
#include <idt.hpp>
#include <multiboot2.hpp>
#include <multiboot_parser.hpp>
#include <serial.hpp>
#include <stddef.h>
#include <stdint.h>
#include <terminal.hpp>
#include <terminal_iterator.hpp>

extern void global_constructor_test();
extern int local_static_variable_test();

// NOLINTNEXTLINE
extern "C" void kernel_main(uint32_t magic, uint32_t addr) {
    const Gdt gdt;
    const Idt idt;

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
            auto meminfo =
                reinterpret_cast<multiboot_tag_basic_meminfo *>(&*tag_it);
            printf("mem_lower = %uKB, mem_upper = %uKB\n", meminfo->mem_lower,
                   meminfo->mem_upper);
        } break;
        case MULTIBOOT_TAG_TYPE_MMAP: {
            auto mmap_tag = reinterpret_cast<multiboot_tag_mmap *>(&*tag_it);
            Multiboot_mmap_iterator mmap_it{mmap_tag};
            Multiboot_mmap_iterator sentinel{mmap_tag + mmap_tag->size / sizeof(multiboot_mmap_entry)};
            while (mmap_it != sentinel) {
                printf(" base_addr = 0x%x%x,"
                       " length = 0x%x%x, type = %s\n",
                       static_cast<uint32_t>(mmap_it->addr >> 32),
                       static_cast<uint32_t>(mmap_it->addr & 0xffffffff),
                       static_cast<uint32_t>(mmap_it->len >> 32),
                       static_cast<uint32_t>(mmap_it->len & 0xffffffff),
                       kstd::to_string(static_cast<MultibootMemoryType>(mmap_it->type)));
                ++mmap_it;
            }
        } break;
        }
        ++tag_it;
    }
    asm volatile("int $0x3");
    asm volatile("int $0x4");
}
