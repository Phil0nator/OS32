#ifndef OS32_TSS
#define OS32_TSS
#include "stdlib/error.h"
#include <stdint.h>

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

err_t __install_tss();
void tss_enter_usermode( void* entrpoint );


#endif