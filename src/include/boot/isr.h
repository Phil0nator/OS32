#ifndef OS32_ISR
#define OS32_ISR
#include <stdint.h>
#include "stdlib/error.h"

// Perform ISR installation
err_t __install_isr();

#endif