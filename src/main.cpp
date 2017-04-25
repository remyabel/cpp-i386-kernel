#include <stddef.h>
#include <stdint.h>

#include <algorithm.hpp>
#include <string.hpp>
#include <basic_string_view.hpp>

#include <Terminal.hpp>
#include <Terminal_iterator.hpp>

#include <gdt.hpp>
#include <idt.hpp>

extern "C"
void kernel_main() {
    for (auto i = 0u; i <= 25; ++i) {
        printf("%d\n", i);
    }

    const Gdt gdt;
    const Idt idt;

    kstd::string_view s{"Initialized GDT"};
    kstd::copy(s.begin(), s.end(), vga::Terminal_output_iterator<char>(vga::terminal, ""));
    printf("\n");

    printf("Initialized IDT\n");

    asm volatile("int $0x3");
    asm volatile("int $0x4");
}
