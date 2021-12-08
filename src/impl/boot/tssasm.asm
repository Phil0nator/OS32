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
; void __umode_iret( void (*x)(), void* stack, int argc, const char** argv, const char* envp )
__umode_iret:
    cli
    push ebp
    mov ebp, esp
    mov ebx, dword[ebp+8]       ; entrypoint
    mov edx, dword[ebp+12]      ; stack
    mov edi, dword[ebp+16]      ; argc
    mov esi, dword[ebp+20]      ; argv
    mov ecx, dword[ebp+24]      ; envp
    mov ax, (5 * 8) | 3
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; (push) parameters for argc, argv, envp
    ; mov [edx-4], edi
    ; mov [edx-8], esi
    ; mov [edx-12], ecx
    ; sub edx, 16
    mov eax, esp
    mov esp, edx
    push ecx
    push esi
    push edi
    push 0
    mov esp, eax

    ; PUSH for entering ring 0
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