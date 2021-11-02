#include "boot/tss.h"
#include "boot/gdt.h"
#include <stdint.h>

#define TSS_GDT_ENTRY (3)

tss_t __tss;
// testing purposes / only 1 process would work
const char* usermode_kernel_stack[4096];
extern void __ltr( uint16_t ent );
extern void __umode_iret( void* e, void* s );
static void flush_tss()
{
    __ltr((TSS_GDT_ENTRY*8) | 3);
}
err_t __install_tss()
{
    // setup a kernel stack
    __tss.esp0 = (uint32_t)usermode_kernel_stack+sizeof(usermode_kernel_stack);
    // setup the correct segment
    __tss.ss0 = 0x10;
    // add the tss to the gdt
    gdt_set_gate( TSS_GDT_ENTRY, (uint32_t) &__tss, sizeof(tss_t), 0x89, 0x40  );
    // load tss to internal register
    flush_tss();
    gdt_flush();
    return OS32_SUCCESS;
}



void tss_enter_usermode( void* entrpoint, tss_t* tss )
{
    // defined in asm
    __tss = *tss;
    __umode_iret( entrpoint, tss->esp );
}