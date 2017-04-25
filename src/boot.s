ALIGN_BIT equ 1<<0
MEMINFO   equ 1<<1
FLAGS     equ ALIGN_BIT | MEMINFO
MAGIC     equ 0x1BADB002
CHECKSUM  equ -(MAGIC + FLAGS)

section .multiboot:
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

section .bss
align 16
stack:
    resb 16384
stack_end:

section .text:
gdtr dw 0 ; size
     dd 0 ; gdt pointer

idtr dw 0
     dd 0

global gdt_set
global idt_set

gdt_set:
    mov eax, [esp + 4]  ; pop ptr into eax
    mov [gdtr + 2], eax ; store ptr into gdtr
    mov ax, [esp + 8]   ; pop size into ax
    mov [gdtr], ax      ; store size into gdtr
    lgdt [gdtr]
    ret

idt_set:
    mov eax, [esp + 4]  ; pop ptr into eax
    mov [idtr + 2], eax ; store ptr into gdtr
    mov ax, [esp + 8]   ; pop size into ax
    mov [idtr], ax      ; store size into gdtr
    lidt [idtr]
    ret

global gdt_flush

gdt_flush:
    mov ax, 0x10      ; 0x10 is the offset in the GDT to our data segment
    mov ds, ax        ; Load all data segment selectors
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp 0x08:.flush   ; 0x08 is the offset to our code segment: Far jump!
.flush:
    ret

global isr0

isr0:
    cli
    push byte 0
    push byte 0
    jmp isr_common_stub

extern fault_handler

isr_common_stub:
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10   ; Load the Kernel Data Segment descriptor!
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp   ; Push us the stack
    push eax
    mov eax, fault_handler
    call eax       ; A special call, preserves the 'eip' register
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8     ; Cleans up the pushed error code and pushed ISR number
    iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!

global _start:function (_start.end - _start)
extern kernel_main

extern _init
extern _fini

_start:
    mov esp, stack_end

    call _init

    call kernel_main

    call _fini

    cli
.loop:
    hlt
    jmp .loop
.end:
