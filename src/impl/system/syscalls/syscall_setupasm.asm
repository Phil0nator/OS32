[bits 32]

section .text
extern __syscall_main
global __syscall_entry

__syscall_entry:
    cli
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp
    push eax
    mov eax, __syscall_main
    call eax
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    sti
    iret