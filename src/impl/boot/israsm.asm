[bits 32]

%macro DECLISR 2
global __isr %+ %1
__isr %+ %1:
    cli
    %if !(%2)
    push byte 0
    %endif
    push byte %1
    ISRCOMMON

%endmacro

%macro ISRCOMMON 0
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
mov eax, __fault_handler
call eax
pop eax
pop gs
pop fs
pop es
pop ds
popa
add esp, 8 
iret
%endmacro

section .text
extern __fault_handler
DECLISR 0, 0
DECLISR 1, 0
DECLISR 2, 0
DECLISR 3, 0
DECLISR 4, 0
DECLISR 5, 0
DECLISR 6, 0
DECLISR 7, 0
DECLISR 8, 1
DECLISR 9, 0
DECLISR 10, 1
DECLISR 11, 1
DECLISR 12, 1
DECLISR 13, 1
DECLISR 14, 1
DECLISR 15, 0
DECLISR 16, 0
DECLISR 17, 0
DECLISR 18, 0
DECLISR 19, 0
DECLISR 20, 0
DECLISR 21, 0
DECLISR 22, 0
DECLISR 23, 0
DECLISR 24, 0
DECLISR 25, 0
DECLISR 26, 0
DECLISR 27, 0
DECLISR 28, 0
DECLISR 29, 0
DECLISR 30, 0
DECLISR 31, 0
