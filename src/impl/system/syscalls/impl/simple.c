#include "impl.h"
#include "system/filesystems/vfs.h"
#include "system/process/multitasking.h"
#include "stdlib/string.h"

int __s_read( int fd, void* buf, size_t count )
{
    return vfs_read( current_process->local_fdt[fd], buf, count );
}
int __s_write( int fd, void* buf, size_t count )
{
    return vfs_write( current_process->local_fdt[fd], buf, count );
}

int __s_getcwd( char* buf, size_t len )
{
    int actual_len = strlen(current_process->wd);
    memcpy( buf, current_process->wd, actual_len > len ? len : actual_len );
    return actual_len;
}
int __s_chdir(const char* path)
{
    struct fstat nullbuf;
    if (vfs_stat( path, &nullbuf ) != OS32_ERROR)
    {
        strcpy(current_process->wd, path);
        return OS32_SUCCESS;
    } 
    return errno;
}
int __s_fchdir( int fd )
{
    struct fstat nullbuf;
    if (vfs_fstat( fd, &nullbuf ) != OS32_ERROR)
    {
        strcpy(current_process->wd, vfs_fpath(fd));
        return OS32_SUCCESS;
    }
    return errno;
}
int __s_fork()
{
    return __fork();
}
int __s_execve( const char* filename, const char** argv, const char* envp )
{
    return __exec( filename, argv, envp );
}