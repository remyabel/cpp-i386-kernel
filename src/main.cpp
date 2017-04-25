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
        char c = static_cast<char>(i) + '0';
        char buf[] = { c, '\n', '\0' };
        vga::terminal.write(buf);
    }

    const Gdt gdt;
    const Idt idt;

    string_view s{"Initialized GDT"};
    copy(s.begin(), s.end(), vga::Terminal_output_iterator<char>(vga::terminal, ""));

    printf("Test printf");
}
