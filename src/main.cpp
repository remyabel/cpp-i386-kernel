#include <stddef.h>
#include <stdint.h>

#include <string.hpp>

#include <Terminal.hpp>

extern "C" /* Use C linkage for kernel_main. */
void kernel_main() {
    Terminal terminal;
	terminal.write("Hello, kernel World!\n");
}
