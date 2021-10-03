#ifndef OS32_IRQ
#define OS32_IRQ
#include <stdint.h>
#include <stddef.h>
#include <stdlib/error.h>
#include "system/interrupt_frame.h"

err_t __install_irq();

typedef void (*irq_routine)( interrupt_frame_t* regs );

err_t irq_install_routine( size_t irqno, irq_routine r );
err_t irq_uninstall_routine( size_t irqno);


#endif