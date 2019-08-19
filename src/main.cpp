#include <stddef.h>
#include <stdint.h>

#include <algorithm.hpp>
#include <basic_string_view.hpp>
#include <string.hpp>

#include <terminal.hpp>
#include <terminal_iterator.hpp>
#include <serial.hpp>

#include <gdt.hpp>
#include <idt.hpp>

#include <multiboot2.hpp>

extern void global_constructor_test();
extern int local_static_variable_test();

extern "C" void kernel_main(uint64_t /*magic*/, uint64_t /*addr*/) {
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

    asm volatile("int $0x3");
    asm volatile("int $0x4");
}
