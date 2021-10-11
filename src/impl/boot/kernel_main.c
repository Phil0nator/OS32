#include "boot/vga.h"
#include "boot/gdt.h"
#include "boot/idt.h"
#include "boot/isr.h"
#include "boot/irq.h"
#include "drivers/timer.h"
#include "stdlib/assert.h"
#include "drivers/keyboard.h"
#include "boot/mutiboot_info.h"
#include "stdlib/kmalloc.h"
#include "boot/page.h"
#include "stdlib/string.h"
#include "tests/kmalloc_unit.h"

#define __kernel_main_hlt while(1);
#define __kernel_main_sti __asm__ __volatile__ ("sti"); 

void testfn()
{
    vgaPrintf("Hello\n");
}

void _kernel_main()
{
    vgaPrintf("%+ Entered Kernel Code\n");

    if ( __load_multiboot() == OS32_ERROR )
    {
        vgaPrintf("%- multiboot load");
        __kernel_main_hlt
    }
    vgaPrintf("%+ Multiboot loaded\n");

    if ( __install_gdt() == OS32_ERROR )
    {
        vgaPrintf("%- GDT setup");
        __kernel_main_hlt
    }
    vgaPrintf("%+ GDT Installed\n");


    if ( __install_idt() == OS32_ERROR )
    {
        vgaPrintf("%- IDT setup");
        __kernel_main_hlt
    }
    vgaPrintf("%+ IDT Installed\n");

    if ( __install_isr() == OS32_ERROR )
    {
        vgaPrintf("%- ISRs setup");
        __kernel_main_hlt
    }
    vgaPrintf("%+ ISRs Installed\n");

    if ( __install_irq() == OS32_ERROR )
    {
        vgaPrintf("%- IRQs setup");
        __kernel_main_hlt
    }
    vgaPrintf("%+ IRQs Installed\n");
    
    if ( __install_timer() == OS32_ERROR )
    {
        vgaPrintf("%- PIT setup");
        __kernel_main_hlt
    }
    vgaPrintf("%+ PIT Installed\n");

    if ( __install_keyboard() == OS32_ERROR )
    {
        vgaPrintf("%- KB setup");
        __kernel_main_hlt
    }
    vgaPrintf("%+ KB Installed\n");

    if ( __install_kmalloc() )
    {
        vgaPrintf("%- kmalloc setup");
        __kernel_main_hlt
    }
    vgaPrintf("%+ kmalloc setup\n");

    idt_update();

    __kernel_main_sti

    __kmalloc_test();

    for(;;);

}

