[bits 32]



section .text

global inportb
global outportb

inportb:
    push edx
    mov edx, [ebp + 8]
    xor eax, eax
    in al, dx
    pop edx
    ret

outportb:
    push edx
    mov edx, [ebp+8]
    mov eax, [ebp+12]
    out dx, al
    pop edx
    ret