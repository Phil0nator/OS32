#ifndef OS32_TSS
#define OS32_TSS
#include "stdlib/error.h"

err_t __install_tss();
void tss_enter_usermode( void* entrpoint );


#endif