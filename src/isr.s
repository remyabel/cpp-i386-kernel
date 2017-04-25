global isr0

isr0:
    cli
    push byte 0
    push byte 0
    jmp isr_common_stub

extern fault_handler

isr_common_stub:
    ; Push general purpose registers
    pushad

    ; Store segments
    push ds
    push es
    push fs
    push gs

    ; Switch to kernel data segment
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Push stack pointer
    push esp

    call fault_handler

    ; Restore stack pointer
    mov esp, eax

    ; Restore segments
    pop gs
    pop fs
    pop es
    pop ds

    ; Restore general purpose registers
    popad

    ; Skip int_no/err_code pushed by ISR handler
    add esp, 8

    ; Restore CS, SS, EIP, ESP, EFFLAGS 
    iret

