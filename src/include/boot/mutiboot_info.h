#ifndef OS32_MBINF
#define OS32_MBINF
#include "external/multiboot.h"
#include "stdlib/error.h"


extern multiboot_info_t __multiboot_info;
// Load info from multiboot into kernelspace
err_t __load_multiboot();



#endif