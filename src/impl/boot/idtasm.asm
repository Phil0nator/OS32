[bits 32]
; http://www.osdever.net/bkerndev/Docs/idt.htm
section .text
global __idt_load
extern __idt_ptr
__idt_load:
    lidt [__idt_ptr]
    ret