section .bss
align 16
stack:
    resb 16384
stack_end:

section .multiboot
align 8
header_start:
    dd 0xe85250d6                ; magic number (multiboot 2)
    dd 0                         ; architecture 0 (protected mode i386)
    dd header_end - header_start ; header length
    ; checksum
    dd -(0xe85250d6 + 0 + (header_end - header_start))

    ; insert optional multiboot tags here

    ; required end tag
    dw 0    ; type
    dw 0    ; flags
    dd 8    ; size
header_end:

section .text
global _start:function (_start.end - _start)
extern kernel_main

extern _init
extern _fini

extern boot_page_directory
extern early_main

_start:
    mov esp, stack_end

    push ebx
    push eax

    call _init

    call early_main

    mov ecx, boot_page_directory
    mov cr3, ecx

    mov ecx, cr0
    or ecx, 0x80000001
    mov cr0, ecx

    call kernel_main

    call _fini

    cli
.loop:
    hlt
    jmp .loop
.end:
