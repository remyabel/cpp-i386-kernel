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


