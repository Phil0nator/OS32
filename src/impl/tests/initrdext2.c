#include "system/filesystems/linitrd.h"
#include "boot/vga.h"
#include "stdlib/kmalloc.h"
#include "system/process/elf.h"
void __initrdext2_test()
{
    char buffer[1024] = {0};
    fd_t fd = vfs_open( "/initrd/test.txt", 0 );
    vfs_read( fd, buffer, 1024 );
    vfs_close( fd );
    vgaPuts(buffer);

}