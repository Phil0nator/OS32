#ifndef OS32_IRQ
#define OS32_IRQ
#include <stdint.h>
#include <stddef.h>
#include <stdlib/error.h>
#include "system/interrupt_frame.h"

/**
 * Perform IRQ installation
 */
err_t __install_irq();

// A routine to be run by interrupts to the irq
typedef void (*irq_routine)( interrupt_frame_t* regs );

/**
 * Add a routine to be executed when a certain IRQ interrupt occurs.
 * @param irqno the irq interrupt number to assign this routine to
 * @param r the routine to add
 */
err_t irq_install_routine( size_t irqno, irq_routine r );
/**
 * Remove an existing routine by its irq number
 */
err_t irq_uninstall_routine( size_t irqno);


#endif