#include <stddef.h>
#include <stdint.h>

#include <algorithm.hpp>
#include <string.hpp>
#include <basic_string_view.hpp>

#include <Terminal.hpp>
#include <Terminal_iterator.hpp>

#include <gdt.hpp>
#include <idt.hpp>

#include <multiboot2.hpp>

extern void global_constructor_test();

extern "C"
void kernel_main(unsigned long magic, unsigned long addr) {
    const Gdt gdt;
    const Idt idt;

    kstd::string_view s{"Initialized GDT"};
    kstd::copy(s.begin(), s.end(), vga::Terminal_output_iterator<char>(vga::terminal, ""));
    printf("\n");

    printf("Initialized IDT\n");

    global_constructor_test();

    asm volatile("int $0x3");
    asm volatile("int $0x4");
}
