#ifndef OS32_FPU
#define OS32_FPU
#include <stdbool.h>
#include "stdlib/error.h"

err_t __install_fpu();

bool fpu_installed();
err_t fpu_reset();

#endif