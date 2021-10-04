[bits 32]



section .text

global inportb
global outportb

inportb:
    push ebp
    mov ebp, esp
    push edx
    mov edx, [ebp+8]
    in al, dx
    pop edx
    leave
    ret

outportb:
    push ebp
    mov ebp, esp
    push edx
    mov edx, [ebp+8]
    mov eax, [ebp+12]
    out dx, al
    pop edx
    leave
    ret