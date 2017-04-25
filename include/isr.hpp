#ifndef ISR_HPP
#define ISR_HPP

#include <array.hpp>
#include <basic_string_view.hpp>

#include <tinyprintf.h>

extern "C" void isr0();
extern "C" void isr1();
extern "C" void isr2();
extern "C" void isr3();
extern "C" void isr4();
extern "C" void isr5();
extern "C" void isr6();
extern "C" void isr7();
extern "C" void isr8();
extern "C" void isr9();
extern "C" void isr10();
extern "C" void isr11();
extern "C" void isr12();
extern "C" void isr13();
extern "C" void isr14();
extern "C" void isr15();
extern "C" void isr16();
extern "C" void isr17();
extern "C" void isr18();
extern "C" void isr19();
extern "C" void isr20();
extern "C" void isr21();
extern "C" void isr22();
extern "C" void isr23();
extern "C" void isr24();
extern "C" void isr25();
extern "C" void isr26();
extern "C" void isr27();
extern "C" void isr28();
extern "C" void isr29();
extern "C" void isr30();
extern "C" void isr31();

struct registers
{
    unsigned int gs, fs, es, ds;      /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
};

constexpr kstd::array<kstd::string_view, 31> exception_msgs = {{
	"Divide by zero exception",
	"Debug exception",
	"Non-maskable interrupt exception",
	"Breakpoint exception",
	"Detected overflow exception",
	"Out of bounds exception",
	"Invalid operation code exception",
	"No coprocessor exception",
	"Double fault exception ",
	"Coprocessor segment overrun exception",
	"Bad TSS exception ",
	"Segment not present exception ",
	"Stack fault exception ",
	"General protection fault exception ",
	"Page fault ",
	"Unknown interrupt exception",
	"Coprocessor fault exception",
	"Alignment check exception",
	"Machine check exception"
}};

extern "C"
void fault_handler(registers *reg) {
    printf("Interrupt called #%d: %s\n", reg->int_no, (exception_msgs.begin() + reg->int_no)->data());

    for (;;);
}

#endif
