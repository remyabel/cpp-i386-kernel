#ifndef IDT_HPP
#define IDT_HPP

#include <stddef.h>
#include <stdint.h>
#include <array.hpp>

#include <Terminal.hpp>

struct idt_entry {
    uint16_t base_low;
    uint16_t selector;
    uint8_t reserved;
    uint8_t flags;
    uint16_t base_high;
} __attribute__((packed));

struct idt_descriptor {
    size_t size;
    idt_entry *pointer;
} __attribute__((packed));

extern "C" void idt_set(idt_entry *, size_t);

extern "C" void isr0();

class Idt {
    array<idt_entry, 256> idt_table;
    idt_descriptor idt_ptr;

    void idt_create_descriptor(size_t index, idt_entry e) {
        *(idt_table.begin() + index) = e;
    }

    void idt_init() {
        idt_ptr.size = (idt_table.size() * sizeof(idt_entry)) - 1; 
        idt_ptr.pointer = idt_table.data();

        uint16_t base_low = reinterpret_cast<uintptr_t>(isr0);
        uint16_t base_high = reinterpret_cast<uintptr_t>(isr0) >> 16;

        idt_create_descriptor(0, { base_low, 0x08, 0, 0x8E, base_high });

        idt_set(idt_ptr.pointer, idt_ptr.size);
    }

public:
    Idt() {
        idt_table = {};
        idt_init();
    }

};

struct registers
{
    unsigned int gs, fs, es, ds;      /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
};

extern "C"
void fault_handler(registers *reg) {
    printf("Fault #%d\n", reg->int_no);

    for (;;);
}

#endif
