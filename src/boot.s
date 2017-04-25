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
