#ifndef GDT_HPP
#define GDT_HPP

#include <stddef.h>
#include <stdint.h>
#include <array.hpp>

#include <Terminal.hpp>

/*
 * This is the Global Descriptor Table
 *
 *  An entry, a "Segment Descriptor", looks like this:
 *
 * 31          24         19   16                 7           0
 * ------------------------------------------------------------
 * |             | |B| |A|       | |   |1|0|E|W|A|            |
 * | BASE 31..24 |G|/|L|V| LIMIT |P|DPL|  TYPE   | BASE 23:16 |  4
 * |             | |D| |L| 19..16| |   |1|1|C|R|A|            |
 * ------------------------------------------------------------
 * |                             |                            |
 * |        BASE 15..0           |       LIMIT 15..0          |  0
 * |                             |                            |
 * ------------------------------------------------------------
 *
 *  Note the ordering of the data items is reversed from the above
 *  description.
 */

struct gdt_entry {
    uint16_t limit_low;   // LIMIT 15..0
    uint16_t base_low;    // BASE  15..0
    uint8_t  base_middle; // BASE  23:16
    uint8_t  access;      // TYPE  3:0, DT 4, DPL 6:5, P 7
    uint8_t  granularity; // LIMIT 19..16, AVL 20, L 21, B/D 22, G 23
    uint8_t  base_high;   // BASE  31..24
} __attribute__((packed));

struct gdt_descriptor {
    size_t size;
    gdt_entry *pointer;
} __attribute__((packed));

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

extern "C" void gdt_set(gdt_entry *, size_t);
extern "C" void gdt_flush();

class Gdt {
    array<gdt_entry, 5> gdt_table;
    gdt_descriptor gdt_ptr;
    
    void gdt_create_descriptor(size_t index, gdt_entry e) {
        *(gdt_table.begin() + index) = e;
    }

    void gdt_init() {
        gdt_ptr.size = (gdt_table.size() * sizeof(gdt_entry)) - 1; 
        gdt_ptr.pointer = gdt_table.data();

        gdt_create_descriptor(0, {}); // null descriptor
        gdt_create_descriptor(1, { 0xFFFF, 0, 0, 0x9A, 0xCF, 0 }); // kernel code segment
        gdt_create_descriptor(2, { 0xFFFF, 0, 0, 0x92, 0xCF, 0 }); // kernel data segment
        gdt_create_descriptor(3, { 0xFFFF, 0, 0, 0xFA, 0xCF, 0 }); // user code segment
        gdt_create_descriptor(4, { 0xFFFF, 0, 0, 0xF2, 0xCF, 0 }); // user data segment

        gdt_set(gdt_ptr.pointer, gdt_ptr.size);
        gdt_flush();
    }

public:
    Gdt() {
        gdt_init();
    }
};

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
    vga::terminal.write("\nFault #");
    auto c = static_cast<char>(reg->int_no + '0');
    char buf[] = { c, '\n', '\0' };
    vga::terminal.write(buf);

    for (;;);
}

#endif
