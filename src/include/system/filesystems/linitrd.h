#ifndef OS32_LINITRD
#define OS32_LINITRD
#include "stdlib/error.h"
#include "vfs.h"
#include "ext2/ext2.h"

extern struct ext2_partition* __initrd;

err_t __install_initrd();


#endif