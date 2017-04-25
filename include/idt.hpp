#ifndef IDT_HPP
#define IDT_HPP

#include <stddef.h>
#include <stdint.h>
#include <array.hpp>

#include <Terminal.hpp>

#include <isr.hpp>

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

class Idt {
    kstd::array<idt_entry, 256> idt_table;
    idt_descriptor idt_ptr;

    void idt_create_descriptor(size_t index, idt_entry e) {
        idt_table[index] = e;
    }

    void idt_create_descriptor(size_t index, void (*isr)()) {
        uint16_t base_low = reinterpret_cast<uintptr_t>(isr);
        uint16_t base_high = reinterpret_cast<uintptr_t>(isr) >> 16;

        idt_create_descriptor(index, { base_low, 0x08, 0, 0x8E, base_high });
    }

    void idt_init() {
        idt_ptr.size = (idt_table.size() * sizeof(idt_entry)) - 1; 
        idt_ptr.pointer = idt_table.data();

        idt_create_descriptor(0, isr0);
        idt_create_descriptor(1, isr1);
        idt_create_descriptor(2, isr2);
        idt_create_descriptor(3, isr3);
        idt_create_descriptor(4, isr4);
        idt_create_descriptor(5, isr5);
        idt_create_descriptor(6, isr6);
        idt_create_descriptor(7, isr7);
        idt_create_descriptor(8, isr8);
        idt_create_descriptor(9, isr9);
        idt_create_descriptor(10, isr10);
        idt_create_descriptor(11, isr11);
        idt_create_descriptor(12, isr12);
        idt_create_descriptor(13, isr13);
        idt_create_descriptor(14, isr14);
        idt_create_descriptor(15, isr15);
        idt_create_descriptor(16, isr16);
        idt_create_descriptor(17, isr17);
        idt_create_descriptor(18, isr18);
        idt_create_descriptor(19, isr19);
        idt_create_descriptor(20, isr20);
        idt_create_descriptor(21, isr21);
        idt_create_descriptor(22, isr22);
        idt_create_descriptor(23, isr23);
        idt_create_descriptor(24, isr24);
        idt_create_descriptor(25, isr25);
        idt_create_descriptor(26, isr26);
        idt_create_descriptor(27, isr27);
        idt_create_descriptor(28, isr28);
        idt_create_descriptor(29, isr29);
        idt_create_descriptor(30, isr30);
        idt_create_descriptor(31, isr31);
        
        idt_set(idt_ptr.pointer, idt_ptr.size);
    }

public:
    Idt() {
        idt_table = {};
        idt_init();
    }

};

#endif
