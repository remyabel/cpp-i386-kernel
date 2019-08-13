; Handle routine macro for interrupts with error code
%macro generate_isr 2
global %1
%1:
	cli
	push %2
	jmp isr_common_stub
%endmacro

; Handle routine macro for interrupts without error code
%macro generate_isr 3
global %1
%1:
	cli
	push %2
	push %3
	jmp isr_common_stub
%endmacro

; Exceptions
generate_isr isr0,  0, 0  ; Divide by zero exception
generate_isr isr1,  0, 1  ; Debug exception
generate_isr isr2,  0, 2  ; Non-maskable interrupt exception
generate_isr isr3,  0, 3  ; Breakpoint exception
generate_isr isr4,  0, 4  ; Detected overflow exception
generate_isr isr5,  0, 5  ; Out of bounds exception
generate_isr isr6,  0, 6  ; Invalid operation code exception
generate_isr isr7,  0, 7  ; No coprocessor exception
generate_isr isr8,  8     ; Double fault exception [has error code]
generate_isr isr9,  0, 9  ; Coprocessor segment overrun exception
generate_isr isr10, 10    ; Bad TSS exception [has error code]
generate_isr isr11, 11    ; Segment not present exception [has error code]
generate_isr isr12, 12    ; Stack fault exception [has error code]
generate_isr isr13, 13    ; General protection fault exception [has error code]
generate_isr isr14, 14    ; Page fault [has error code]
generate_isr isr15, 0, 15 ; Unknown interrupt exception
generate_isr isr16, 0, 16 ; Coprocessor fault exception
generate_isr isr17, 0, 17 ; Alignment check exception
generate_isr isr18, 0, 18 ; Machine check exception

; Reserved exceptions
generate_isr isr19, 0, 19
generate_isr isr20, 0, 20
generate_isr isr21, 0, 21
generate_isr isr22, 0, 22
generate_isr isr23, 0, 23
generate_isr isr24, 0, 24
generate_isr isr25, 0, 25
generate_isr isr26, 0, 26
generate_isr isr27, 0, 27
generate_isr isr28, 0, 28
generate_isr isr29, 0, 29
generate_isr isr30, 0, 30
generate_isr isr31, 0, 31

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

