[bits 32]


section .text
global __ltr
global __umode_iret
; void __ltr( uint16_t addr )
__ltr:
    push ebp
    mov ebp, esp
    mov ax, [ebp+8]
    ; set the task register to the given tss
    ltr ax
    leave
    ret

; https://wiki.osdev.org/Getting_to_Ring_3
; void __umode_iret( void (*x)(), void* stack )
__umode_iret:
    cli
    push ebp
    mov ebp, esp
    mov ebx, dword[ebp+8]
    mov edx, dword[ebp+12]
    mov ax, (5 * 8) | 3
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; mov eax, esp
    push (5 * 8) | 3
    push edx
    pushf
    pop eax
    or eax, 0x200
    push eax
    push (4 * 8) | 3
    push ebx
    iret
    leave
    ret