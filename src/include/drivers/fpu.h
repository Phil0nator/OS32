#ifndef OS32_FPU
#define OS32_FPU
#include <stdbool.h>
#include "stdlib/error.h"

/**
 * Perform FPU installation and setup
 */
err_t __install_fpu();

// Check if the FPU is installed
bool fpu_installed();
// Reset the state of the FPU
err_t fpu_reset();

#endif