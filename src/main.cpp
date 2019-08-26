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
    Multiboot_tag_iterator it{addr + 8};
    while (it->type != MULTIBOOT_TAG_TYPE_END) {
        switch (it->type) {
        case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
            auto meminfo =
                reinterpret_cast<multiboot_tag_basic_meminfo *>(&*it);
            printf("mem_lower = %uKB, mem_upper = %uKB\n", meminfo->mem_lower,
                   meminfo->mem_upper);
            break;
        }
        ++it;
    }
    asm volatile("int $0x3");
    asm volatile("int $0x4");
}
