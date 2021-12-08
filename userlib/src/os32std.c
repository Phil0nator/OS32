#include "os32std.h"
#include <stdint.h>
#include "sys/syscall.h"
int __errno;
int __get_errno()
{
    return __errno;
}


int read( int fd, void* buf, size_t count )
{
    return errno_wrap( __do_syscall( SYSNO(read), fd, (uint32_t)buf, (uint32_t)count, 0,0,0 ));
}
int write( int fd, const void* buf, size_t count )
{
    return errno_wrap( __do_syscall( SYSNO(write), fd, (uint32_t)buf, (uint32_t)count, 0, 0, 0 ));
}
int getcwd(char* buf, size_t len)
{
    return errno_wrap( __do_syscall( SYSNO(getcwd), buf, len,0,0,0,0 ));
}
int chdir( const char* path )
{
    return errno_wrap( __do_syscall( SYSNO(chdir),path,0,0,0,0,0 ));
}
int fchdir( int fd )
{
    return errno_wrap( __do_syscall( SYSNO(fchdir), fd, 0,0,0,0,0 ));
}
int fork()
{
    return errno_wrap( __do_syscall( SYSNO(fork), 0,0,0,0,0,0 ));
}
int execve( const char* filename, const char** argv, const char* envp )
{
    return errno_wrap( __do_syscall( SYSNO(execve), filename, argv, envp, 0, 0, 0 ));
}

void _exit(int status)
{
    return __do_syscall( SYSNO(exit), status, 0 , 0, 0 , 0, 0 );
}