#include "boot/isr.h"
#include "boot/idt.h"
#include "system/interrupt_frame.h"
#include "boot/vga.h"
// http://www.osdever.net/bkerndev/Docs/isrs.htm
static const char *exception_messages[] =
{
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

extern void __isr0();
extern void __isr1();
extern void __isr2();
extern void __isr3();
extern void __isr4();
extern void __isr5();
extern void __isr6();
extern void __isr7();
extern void __isr8();
extern void __isr9();
extern void __isr10();
extern void __isr11();
extern void __isr12();
extern void __isr13();
extern void __isr14();
extern void __isr15();
extern void __isr16();
extern void __isr17();
extern void __isr18();
extern void __isr19();
extern void __isr20();
extern void __isr21();
extern void __isr22();
extern void __isr23();
extern void __isr24();
extern void __isr25();
extern void __isr26();
extern void __isr27();
extern void __isr28();
extern void __isr29();
extern void __isr30();
extern void __isr31();



err_t __install_isr()
{

    //          gateno  function   segment, perms + required #14
    idt_set_gate( 0, (size_t)__isr0, 0x08, 0x8E );
    idt_set_gate( 1, (size_t)__isr1, 0x08, 0x8E );
    idt_set_gate( 2, (size_t)__isr2, 0x08, 0x8E );
    idt_set_gate( 3, (size_t)__isr3, 0x08, 0x8E );
    idt_set_gate( 4, (size_t)__isr4, 0x08, 0x8E );
    idt_set_gate( 5, (size_t)__isr5, 0x08, 0x8E );
    idt_set_gate( 6, (size_t)__isr6, 0x08, 0x8E );
    idt_set_gate( 7, (size_t)__isr7, 0x08, 0x8E );
    idt_set_gate( 8, (size_t)__isr8, 0x08, 0x8E );
    idt_set_gate( 9, (size_t)__isr9, 0x08, 0x8E );
    idt_set_gate( 10, (size_t)__isr10, 0x08, 0x8E );
    idt_set_gate( 11, (size_t)__isr11, 0x08, 0x8E );
    idt_set_gate( 12, (size_t)__isr12, 0x08, 0x8E );
    idt_set_gate( 13, (size_t)__isr13, 0x08, 0x8E );
    idt_set_gate( 14, (size_t)__isr14, 0x08, 0x8E );
    idt_set_gate( 15, (size_t)__isr15, 0x08, 0x8E );
    idt_set_gate( 16, (size_t)__isr16, 0x08, 0x8E );
    idt_set_gate( 17, (size_t)__isr17, 0x08, 0x8E );
    idt_set_gate( 18, (size_t)__isr18, 0x08, 0x8E );
    idt_set_gate( 19, (size_t)__isr19, 0x08, 0x8E );
    idt_set_gate( 20, (size_t)__isr20, 0x08, 0x8E );
    idt_set_gate( 21, (size_t)__isr21, 0x08, 0x8E );
    idt_set_gate( 22, (size_t)__isr22, 0x08, 0x8E );
    idt_set_gate( 23, (size_t)__isr23, 0x08, 0x8E );
    idt_set_gate( 24, (size_t)__isr24, 0x08, 0x8E );
    idt_set_gate( 25, (size_t)__isr25, 0x08, 0x8E );
    idt_set_gate( 26, (size_t)__isr26, 0x08, 0x8E );
    idt_set_gate( 27, (size_t)__isr27, 0x08, 0x8E );
    idt_set_gate( 28, (size_t)__isr28, 0x08, 0x8E );
    idt_set_gate( 29, (size_t)__isr29, 0x08, 0x8E );
    idt_set_gate( 30, (size_t)__isr30, 0x08, 0x8E );
    idt_set_gate( 31, (size_t)__isr31, 0x08, 0x8E );
    return OS32_SUCCESS;
}


void __fault_handler( interrupt_frame_t* regs )
{
    if (regs->intno < 32)
    {
        vgaPrintf("%- %s: %i", exception_messages[regs->intno], regs->err);
        for(;;);
    }
}