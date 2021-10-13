#include "boot/tss.h"
#include "boot/gdt.h"
#include <stdint.h>

#define TSS_GDT_ENTRY (3)
#pragma pack(1)
typedef struct tss_struct
{
    uint16_t link, reserved;
    uint32_t esp0;
    uint16_t ss0, reserved_;
    uint32_t esp1;
    uint16_t ss1, reserved__;
    uint32_t esp2;
    uint16_t ss2, reserved___;
    uint32_t cr3;
    uint32_t eip;
    uint32_t eflags;
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    uint16_t es, reserved____;
    uint16_t cs, reserved_____;
    uint16_t ss, reserved______;
    uint16_t ds, reserved_______;
    uint16_t fs, reserved________;
    uint16_t gs, reserved_________;
    uint16_t ldtr, reserved__________;
    uint16_t reserved___________, IOBP;
} tss_t;
#pragma pack(0)

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