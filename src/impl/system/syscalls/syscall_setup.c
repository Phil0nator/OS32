#include "system/syscalls/syscall_setup.h"
#include "system/syscalls/syscall.h"
#include "boot/idt.h"
#include "system/interrupt_frame.h"
#include "boot/vga.h"
#include "boot/irq.h"
#include "syscall_registry.h"
extern void __syscall_entry();
void __syscall_main( interrupt_frame_t* regs )
{
    // vgaPrintf("%+ SYSCALL MADE!");
    // return;
    __os32_syscall_registry[ regs->eax ]( regs->ebx, regs->ecx, regs->edx, regs->esi, regs->edi, regs->ebp );
}

err_t __install_syscalls()
{
    idt_set_gate( 0x80, (uint32_t)__syscall_entry, 0x08, 0x8E | 0x60 );
    // irq_install_routine( 0x80, (irq_routine) __syscall_main );
    return OS32_SUCCESS;
}