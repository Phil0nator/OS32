[bits 32]


; void set_cr3( phys_addr a );
; phys_addr get_cr3();

section .text
global set_cr3
global get_cr3
global __invlpg_flush
set_cr3:
    push ebp
    mov ebp, esp
    mov eax, [ebp+8]
    mov cr3, eax
    leave
    ret

get_cr3:
    push ebp
    mov ebp, esp
    mov eax, cr3
    leave
    ret

__invlpg_flush:
    invlpg [0]
    ret