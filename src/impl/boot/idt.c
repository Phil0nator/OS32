#include "boot/idt.h"
#include "stdlib/string.h"
#include <stddef.h>
// http://www.osdever.net/bkerndev/Docs/idt.htm
#pragma pack(1)
struct idt_entry
{
    uint16_t base_lo;
    uint16_t sel;
    uint8_t reserved;
    uint8_t flags;
    uint16_t base_hi;
};

struct idt_ptr
{
    uint16_t limit;
    uint32_t base;
};
#pragma pack(0)


struct idt_entry __idt[256] __attribute__((aligned (0x10)));
struct idt_ptr __idt_ptr;

extern void __idt_load();


err_t idt_set_gate(uint8_t num, size_t base, uint16_t sel, uint8_t flags)
{
    // Setup the routine's base address
    __idt[num].base_lo = (base & 0xFFFF);
    __idt[num].base_hi = (base >> 16);

    // set the segment 'selector' that the entry will use
    __idt[num].sel = sel;
    __idt[num].reserved = 0;
    __idt[num].flags = flags;
    return OS32_SUCCESS;
}

err_t __install_idt()
{
    __idt_ptr.limit = (sizeof(struct idt_entry) * 256) -1;
    __idt_ptr.base = (uint32_t) &__idt;

    memset(&__idt, 0, sizeof(__idt));


    __idt_load();
    return OS32_SUCCESS;
}

void idt_update()
{
    __idt_load();
}