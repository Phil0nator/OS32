#include "boot/tss.h"
#include "boot/gdt.h"
#include <stdint.h>

#define TSS_GDT_ENTRY (3)

tss_t __tss;
const char* usermode_kernel_stack[4096];
extern void __ltr( uint16_t ent );
extern void __umode_iret( void* );

err_t __install_tss()
{
    __tss.esp0 = (uint32_t)usermode_kernel_stack+sizeof(usermode_kernel_stack);
    __tss.ss0 = 0x10;
    gdt_set_gate( TSS_GDT_ENTRY, (uint32_t) &__tss, sizeof(tss_t), 0x89, 0x40  );
    __ltr( TSS_GDT_ENTRY*8 );
    return OS32_SUCCESS;
}



void tss_enter_usermode( void* entrpoint )
{
    __umode_iret( entrpoint );
}