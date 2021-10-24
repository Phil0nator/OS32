[bits 32]

section .text
global cpuid_get_features
cpuid_get_features:
    push ebp
    mov ebp, esp
    push edx
    push ecx
    push ebx
    push eax
    mov eax, 1
    ; call cpuid
    cpuid
    ; copy the values into the structure given from the
    ; C code. See cpuid.h for the definition
    mov eax, [ebp+8]
    mov [eax], edx
    mov [eax+4], ecx
    pop eax
    pop ebx
    pop ecx
    pop edx
    leave
    ret