#ifndef OS32_STACK_FRAME
#define OS32_STACK_FRAME
#include <stdint.h>
#include <stddef.h>
#pragma pack(1)

typedef struct interrupt_frame
{
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t intno, err;
    uint32_t eip, cs, eflags, useresp, ss;
} interrupt_frame_t;

#pragma pack(0)

#endif