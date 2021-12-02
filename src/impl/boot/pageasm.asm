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



; https://web.archive.org/web/20160326122214/http://jamesmolloy.co.uk/tutorial_html/9.-Multitasking.html
global cpy_phys_pgs
extern __get_eip
cpy_phys_pgs:

    cli
    mov edi, [ebp+4]
    mov esi, [ebp+8]
    pushf

    mov edx, cr3
    xor eax, eax
    mov cr3, eax
    jmp (.not_paged - 0xc0000000)
    .not_paged:

    mov ecx, 1024

    .lp:

        mov eax, [esi]
        mov [edi], eax
        add edi, 4
        add esi, 4
        loop .lp

    mov edx, cr0
    and edx, 0x80000000
    mov cr0, edx

    popf
    ret
