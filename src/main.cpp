#include <stddef.h>
#include <stdint.h>

/* Hardware text mode color constants. */
enum class color : uint8_t {
	black = 0,
	blue = 1,
	green = 2,
	cyan = 3,
	red = 4,
	magenta = 5,
	brown = 6,
	light_grey = 7,
	dark_grey = 8,
	light_blue = 9,
	light_green = 10,
	light_cyan = 11,
	light_red = 12,
	light_magenta = 13,
	light_brown = 14,
	white = 15,
};
 
color make_color(color fg, color bg) {
	return static_cast<color>(
               static_cast<uint8_t>(fg) 
            | (static_cast<uint8_t>(bg) << 4)
        );
}
 
uint16_t make_colored_char(unsigned char uc, color c) {
	return (uint16_t) uc | static_cast<uint16_t>(c) << 8;
}
 
size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}
 
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
 
size_t terminal_row;
size_t terminal_column;
color terminal_color;
uint16_t* terminal_buffer;
 
void terminal_initialize(void) {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = make_color(color::light_grey, color::black);
	terminal_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = make_colored_char(' ', terminal_color);
		}
	}
}
 
void terminal_setcolor(color color) {
	terminal_color = color;
}
 
void terminal_putentryat(char c, color color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = make_colored_char(c, color);
}
 
void terminal_putchar(char c) {
	terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
			terminal_row = 0;
	}
}
 
void terminal_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}
 
void terminal_writestring(const char* data) {
	terminal_write(data, strlen(data));
}
 
extern "C" /* Use C linkage for kernel_main. */
void kernel_main() {
	/* Initialize terminal interface */
	terminal_initialize();
 
	/* Newline support is left as an exercise. */
	terminal_writestring("Hello, kernel World!\n");
}
