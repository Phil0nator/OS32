#include "impl.h"
#include "system/filesystems/vfs.h"
#include "system/process/multitasking.h"

int __s_read( int fd, void* buf, size_t count )
{
    return vfs_read( current_process->local_fdt[fd], buf, count );
}
int __s_write( int fd, void* buf, size_t count )
{
    return vfs_write( current_process->local_fdt[fd], buf, count );
}