#ifndef OS32_MBINF
#define OS32_MBINF
#include "external/multiboot.h"
#include "stdlib/error.h"


extern multiboot_info_t __multiboot_info;

err_t __load_multiboot();



#endif