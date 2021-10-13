#include "boot/gdt.h"
#include "boot/vga.h"
#pragma pack(1)
// http://www.osdever.net/bkerndev/Docs/gdt.htm
struct gdt_entry
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
};


struct gdt_ptr
{
    uint16_t limit;
    uint32_t base;
};

#pragma pack(0)

struct gdt_entry __gdt[8];
struct gdt_ptr __gdt_ptr;


err_t gdt_set_gate( 
    int32_t number, 
    unsigned long base, 
    unsigned long limit, 
    uint8_t access, 
    uint8_t granularity 
)
{
    //http://www.osdever.net/bkerndev/Docs/gdt.htm
    __gdt[number].base_low = (base & 0xFFFF);
    __gdt[number].base_middle = (base >> 16) & 0xFF;
    __gdt[number].base_high = (base >> 24) & 0xFF;

    __gdt[number].limit_low = (limit & 0xFFFF);
    __gdt[number].granularity = ((limit >> 16) & 0x0F);

    __gdt[number].granularity |= (granularity & 0xF0);
    __gdt[number].access = access;

    return OS32_SUCCESS;
}

err_t __install_gdt()
{
    __gdt_ptr.limit = (sizeof(__gdt))-1;
    __gdt_ptr.base = (uint32_t) &__gdt;

    gdt_set_gate( 0,0,0,0,0 ); // NUll entry
    gdt_set_gate( 1, 0, 0xFFFFFFFF, 0x9A, 0xCF ); // kernel code
    gdt_set_gate( 2, 0, 0xFFFFFFFF, 0x92, 0xCF ); // kernel data
    gdt_set_gate( 4, 0, 0xFFFFFFFF, 0x9A | (((3) &  0x03) << 0x05), 0xCF ); // user code
    gdt_set_gate( 5, 0, 0xFFFFFFFF, 0x92 | (((3) &  0x03) << 0x05), 0xCF ); // user data
    

    gdt_flush();
    return OS32_SUCCESS;
}