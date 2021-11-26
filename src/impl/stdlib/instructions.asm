[bits 32]


section .text
global __get_eip
__get_eip:
    pop eax
    jmp eax
