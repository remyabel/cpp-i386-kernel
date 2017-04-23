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

global gdt_set

gdt_set:
    mov eax, [esp + 4]  ; pop ptr into eax
    mov [gdtr + 2], eax ; store ptr into gdtr
    mov ax, [esp + 8]   ; pop size into ax
    mov [gdtr], ax      ; store size into gdtr
    lgdt [gdtr]
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
