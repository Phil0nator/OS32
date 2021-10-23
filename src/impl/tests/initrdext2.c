#include "system/filesystems/linitrd.h"
#include "boot/vga.h"
void __initrdext2_test()
{
    char buffer[1024] = {0};
    fd_t fd = ext2_open( __initrd, "/test.txt" );
    ext2_read( __initrd, fd, buffer, 1024, 0 );
    vgaPuts( buffer );
}