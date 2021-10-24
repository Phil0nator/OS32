#include "system/filesystems/linitrd.h"
#include "boot/vga.h"
#include "stdlib/kmalloc.h"
#include "system/process/elf.h"
void __initrdext2_test()
{
    char buffer[1024] = {0};
    // fd_t fd = ext2_open( __initrd, "/test.txt" );
    // ext2_read( __initrd, fd, buffer, 1024, 0 );
    // vgaPuts( buffer );
    // ext2_close( __initrd, fd );
    fd_t fd = vfs_open( "/initrd/test.txt", 0 );
    vfs_read( fd, buffer, 1024 );
    vfs_close( fd );
    vgaPuts(buffer);

    fd = vfs_open("/initrd/bin/test", 0);
    vfs_seekg( fd, 0, VFS_SEEK_END );
    size_t size = vfs_tellg(fd);
    vfs_seekg( fd, 0, VFS_SEEK_SET );
    char* data = kmalloc(size);
    vfs_read( fd, data, size );
    elf_load_for_exec( data );
    kfree(data);
    vfs_close(fd);

}