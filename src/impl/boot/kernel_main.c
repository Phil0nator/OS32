#include "boot/vga.h"
#include "boot/gdt.h"
#include "boot/idt.h"
#include "boot/isr.h"
#include "boot/irq.h"

#define __kernel_main_hlt while(1);
#define __kernel_main_sti __asm__ __volatile__ ("sti"); 

void testfn()
{
    vgaPrintf("Hello\n");
}

void _kernel_main()
{
    vgaPrintf("%+ Entered Kernel Code\n");
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

    idt_update();
    // irq_install_routine( 0, (void*)testfn );

    __kernel_main_sti

    for(;;);

}

