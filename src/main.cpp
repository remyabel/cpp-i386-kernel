#include <stddef.h>
#include <stdint.h>

#include <string.hpp>

#include <Terminal.hpp>

#include <gdt.hpp>

extern "C" /* Use C linkage for kernel_main. */
void kernel_main() {
    Terminal terminal;
    for (auto i = 0u; i <= 25; ++i) {
        char c = static_cast<char>(i) + '0';
        char buf[] = { c, '\n', '\0' };
        terminal.write(buf);
    }

    gdt_init();

    terminal.write("Initialized GDT");
}
