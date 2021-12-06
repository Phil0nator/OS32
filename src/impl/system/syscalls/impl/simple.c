#include "impl.h"
#include "system/filesystems/vfs.h"
#include "system/process/multitasking.h"
#include "stdlib/string.h"

#define CONCAFILENAME(path) char concatbuf[VFS_MAX_PATH];\    
    char* pathptr = concatbuf;\
    if (path[0] == '/')\
    {\
        pathptr = path;\
    }\
    else\
    {\
        strcpy(concatbuf, current_process->wd);\
        strcat(concatbuf, path);\
    }\

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
    __set_errno(OS32_SUCCESS);
    struct fstat nullbuf;
    CONCAFILENAME(path)
    if (vfs_stat( pathptr, &nullbuf ) != OS32_ERROR)
    {
        strcpy(current_process->wd, pathptr);
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
    __set_errno(OS32_SUCCESS);
    __exec( filename, argv, envp );
    return errno; 
}
int __s_stat( const char* filename, struct fstat* buf )
{
    __set_errno(OS32_SUCCESS);
    CONCAFILENAME(filename)
    vfs_stat( pathptr, buf );
    return errno;
}
int __s_fstat( int fd, struct fstat* buf )
{
    __set_errno(OS32_SUCCESS);
    vfs_fstat( current_process->local_fdt[fd], buf );
    return errno; 
}
int __s_lstat( const char* filename, struct fstat* buf )
{
    __set_errno(OS32_SUCCESS);
    CONCAFILENAME(filename)
    vfs_lstat( pathptr, buf );
    return errno; 
}
