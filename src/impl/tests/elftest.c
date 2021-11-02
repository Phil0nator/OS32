#include "tests/elftest.h"
#include "system/filesystems/linitrd.h"
#include "system/filesystems/vfs.h"
#include "stdlib/kmalloc.h"
#include "system/process/elf.h"
#include "stdlib/string.h"


void __elf_test()
{
    fd_t fd = vfs_open( "/initrd/bin/test", 0);
    if (fd < 0)
    {
        kpanic("could not open test.elf");
    }
    vfs_seekg( fd, 0, VFS_SEEK_END );
    size_t len = vfs_tellg(fd);
    vfs_seekg( fd, 0, VFS_SEEK_SET );
    char* data = kmalloc( len );
    vfs_read(fd, data, len);
    struct elf_file* elf = elf_load( data );
    process_t proc;
    bzero(&proc, sizeof(process_t));
    process_create(&proc);

    void (*entry)() = elf_load_for_exec( elf, &proc );
    process_start(&proc, entry);

    process_destroy(&proc);
    elf_free(elf);
    kfree(data);
    vfs_close(fd);
}