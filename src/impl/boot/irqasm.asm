[bits 32]

; declare an irq function
; parameter 1: irq number
; parameter 2: does it require a dummy value on the stack
%macro DECLIRQ 2
global __irq %+ %1
__irq %+ %1:
    ; cli
    %if !(%2)
    push byte 0
    %endif
    push byte %1+32
    IRQCOMMON

%endmacro

; common body shared between all irq functions
%macro IRQCOMMON 0
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
mov eax, __irq_handler
call eax
pop eax
pop gs
pop fs
pop es
pop ds
popa
add esp, 8 
sti
iret
%endmacro

section .text
extern __irq_handler
DECLIRQ 0, 0
DECLIRQ 1, 0
DECLIRQ 2, 0
DECLIRQ 3, 0
DECLIRQ 4, 0
DECLIRQ 5, 0
DECLIRQ 6, 0
DECLIRQ 7, 0
DECLIRQ 8, 0
DECLIRQ 9, 0
DECLIRQ 10, 0
DECLIRQ 11, 0
DECLIRQ 12, 0
DECLIRQ 13, 0
DECLIRQ 14, 0
DECLIRQ 15, 0

DECLIRQ 128, 0