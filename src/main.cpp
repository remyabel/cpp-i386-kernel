#include <stddef.h>
#include <stdint.h>

#include <string.hpp>

#include <Terminal.hpp>

extern "C" /* Use C linkage for kernel_main. */
void kernel_main() {
    Terminal terminal;
    for (auto i = 0u; i <= 25; ++i) {
        char c = static_cast<char>(i) + '0';
        char buf[] = { c, '\n', '\0' };
        terminal.write(buf);
    }

    terminal.write("This is the last line");
}
