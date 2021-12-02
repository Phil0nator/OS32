[bits 32]

%define ALIGN (1)
%define MEMINFO  (2)
%define MODS (8)
%define MBFLAGS  (ALIGN | MEMINFO)
%define MAGIC  (0x1BADB002)
%define CHECKSUM  (-(MAGIC + MBFLAGS))
%define ADDROFF 0xC0000000

section .multiboot.data
align 4
dd MAGIC
dd MBFLAGS
dd CHECKSUM

section .bootstrap_stack, NOBITS
global __bootstack_top
stack_bottom: 
resb 16384
__bootstack_top:

section .data
align 4096
global boot_page_directory
; Hardcoded id-mapped page tables
boot_page_directory:
dd 0x00000083
dd 0x00400083
dd 0x00800083
dd 0x00c00083
dd 0x01000083
dd 0x01400083
times (768-6) dd 0
dd 0x00000083
dd 0x00400083
dd 0x00800083
dd 0x00c00083
dd 0x01000083
dd 0x01400083
times (1024-768) dd 0
times (1024) dd 0
dd boot_page_directory - ADDROFF ; physical adrress

section .text
extern _kernel_start
extern _kernel_end
extern _kernel_main
extern __multiboot_info_temporary
global _boot
_boot:
    ; load page directory to cr3
    mov ecx, boot_page_directory - ADDROFF
    mov cr3, ecx

    mov ecx, cr4
    or ecx, 0x00000010
    mov cr4, ecx

    mov ecx, cr0
    or ecx, 0x80000000
    mov cr0, ecx

    ; jump to higher half
    lea ecx, [_boot_paged]
    jmp ecx

section .text
_boot_paged:
    ; remove id-map of nullptr
    mov dword[boot_page_directory], 0

    ; add offset to temporary multiboot pointer
    ; so that it can be accessed from a higher-half paged context
    add ebx, ADDROFF
    mov [__multiboot_info_temporary], ebx

    ; move to C code
    ; see kernel_main.c
    mov esp, __bootstack_top
    call _kernel_main

    ; This should never be reached
    cli
    hlt
    jmp $



