#ifndef OS32_SYSCALL_IMPL
#define OS32_SYSCALL_IMPL
#include <stddef.h>
#include "system/filesystems/vfs.h"

int __s_read( int fd, void* buf, size_t count );
int __s_write( int fd, void* buf, size_t count );
int __s_getcwd( char* buf, size_t len );
int __s_chdir(const char* path);
int __s_fchdir( int fd );
int __s_fork();
int __s_execve( const char* filename, const char** argv, const char* envp );
int __s_stat( const char* filename, struct fstat* buf );
int __s_fstat( int fd, struct fstat* buf );
int __s_lstat( const char* filename, struct fstat* buf );


#endif