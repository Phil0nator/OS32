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
stack_bottom: 
resb 16384
stack_top:

section .data
align 4096
global boot_page_directory
boot_page_directory:
dd 0x00000083
dd 0x00400083
dd 0x00800083
dd 0x00c00083
times (768-4) dd 0
dd 0x00000083
dd 0x00400083
dd 0x00800083
dd 0x00c00083
times (1024-768) dd 0
times (1024) dd 0

section .text
extern _kernel_start
extern _kernel_end
extern _kernel_main
extern __multiboot_info_temporary
global _boot
_boot:
    mov ecx, boot_page_directory - ADDROFF
    mov cr3, ecx

    mov ecx, cr4
    or ecx, 0x00000010
    mov cr4, ecx

    mov ecx, cr0
    or ecx, 0x80000000
    mov cr0, ecx
    lea ecx, [_boot_paged]
    jmp ecx

section .text
_boot_paged:

    mov dword[boot_page_directory], 0

    add ebx, ADDROFF
    mov [__multiboot_info_temporary], ebx

    mov esp, stack_top
    call _kernel_main


    cli
    hlt
    jmp $



