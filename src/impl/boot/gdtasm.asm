[bits 32]
; http://www.osdever.net/bkerndev/Docs/gdt.htm
section .text
global gdt_flush
extern __gdt_ptr
gdt_flush:
    lgdt [__gdt_ptr] ; load special pointer
    mov ax, 0x10     ; 0x10 is offset for GDT to data
    ; update segment registers
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:flush2

flush2:
    ret
