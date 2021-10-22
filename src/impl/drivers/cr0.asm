[bits 32]


section .text
;uint32_t cr0_get();
;void cr0_set( uint32_t cr0 );
global cr0_get
global cr0_set

cr0_get:
    mov eax, cr0
    ret

cr0_set:
    push ebp
    mov ebp, esp
    mov eax, [ebp+8]
    mov cr0, eax
    leave
    ret
