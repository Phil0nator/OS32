#include "system/filesystems/linitrd.h"

void __initrdext2_test()
{
    ext2_open( __initrd, "/test.txt" );
}