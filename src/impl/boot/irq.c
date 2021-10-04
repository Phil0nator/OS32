#include "boot/irq.h"
#include "boot/idt.h"
#include "system/interrupt_frame.h"
#include "stdlib/ioinstrs.h"
#include "boot/vga.h"
//http://www.osdever.net/bkerndev/Docs/irqs.htm

extern void __irq0();
extern void __irq1();
extern void __irq2();
extern void __irq3();
extern void __irq4();
extern void __irq5();
extern void __irq6();
extern void __irq7();
extern void __irq8();
extern void __irq9();
extern void __irq10();
extern void __irq11();
extern void __irq12();
extern void __irq13();
extern void __irq14();
extern void __irq15();
extern void __irq16();
extern void __irq17();
extern void __irq18();
extern void __irq19();
extern void __irq20();
extern void __irq21();
extern void __irq22();
extern void __irq23();
extern void __irq24();
extern void __irq25();
extern void __irq26();
extern void __irq27();
extern void __irq28();
extern void __irq29();
extern void __irq30();
extern void __irq31();

static irq_routine irq_routines[16] = {NULL};


err_t irq_install_routine( size_t irqno, irq_routine r )
{
    irq_routines[irqno] = r;
    return OS32_SUCCESS;
}
err_t irq_uninstall_routine( size_t irqno)
{
    irq_routines[irqno] = NULL;
    return OS32_SUCCESS;
}


/* Normally, IRQs 0 to 7 are mapped to entries 8 to 15. This
*  is a problem in protected mode, because IDT entry 8 is a
*  Double Fault! Without remapping, every time IRQ0 fires,
*  you get a Double Fault Exception, which is NOT actually
*  what's happening. We send commands to the Programmable
*  Interrupt Controller (PICs - also called the 8259's) in
*  order to make IRQ0 to 15 be remapped to IDT entries 32 to
*  47 
* http://www.osdever.net/bkerndev/Docs/irqs.htm
*/
static void irq_remap(void)
{
    outportb(0x20, 0x11);
    outportb(0xA0, 0x11);
    outportb(0x21, 0x20);
    outportb(0xA1, 0x28);
    outportb(0x21, 0x04);
    outportb(0xA1, 0x02);
    outportb(0x21, 0x01);
    outportb(0xA1, 0x01);
    outportb(0x21, 0x0);
    outportb(0xA1, 0x0);
}

err_t __install_irq()
{
    irq_remap();

    idt_set_gate( 32, (size_t)__irq0, 0x08, 0x8E );
    idt_set_gate( 33, (size_t)__irq1, 0x08, 0x8E );
    idt_set_gate( 34, (size_t)__irq2, 0x08, 0x8E );
    idt_set_gate( 35, (size_t)__irq3, 0x08, 0x8E );
    idt_set_gate( 36, (size_t)__irq4, 0x08, 0x8E );
    idt_set_gate( 37, (size_t)__irq5, 0x08, 0x8E );
    idt_set_gate( 38, (size_t)__irq6, 0x08, 0x8E );
    idt_set_gate( 39, (size_t)__irq7, 0x08, 0x8E );
    idt_set_gate( 40, (size_t)__irq8, 0x08, 0x8E );
    idt_set_gate( 41, (size_t)__irq9, 0x08, 0x8E );
    idt_set_gate( 42, (size_t)__irq10, 0x08, 0x8E );
    idt_set_gate( 43, (size_t)__irq11, 0x08, 0x8E );
    idt_set_gate( 44, (size_t)__irq12, 0x08, 0x8E );
    idt_set_gate( 45, (size_t)__irq13, 0x08, 0x8E );
    idt_set_gate( 46, (size_t)__irq14, 0x08, 0x8E );
    idt_set_gate( 47, (size_t)__irq15, 0x08, 0x8E );
    return OS32_SUCCESS;

}

void __irq_handler( interrupt_frame_t* regs )
{
    if ( irq_routines[ regs->intno-32 ] )
    {
        irq_routines[ regs->intno-32 ](regs);
    }

    // http://www.osdever.net/bkerndev/Docs/irqs.htm

    /* If the IDT entry that was invoked was greater than 40
    *  (meaning IRQ8 - 15), then we need to send an EOI to
    *  the slave controller */
    if (regs->intno >= 40)
    {
        outportb(0xA0, 0x20);
    }

    /* In either case, we need to send an EOI to the master
    *  interrupt controller too */
    outportb(0x20, 0x20);

}