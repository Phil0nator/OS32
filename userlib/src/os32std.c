#include "os32std.h"
#include "system/syscalls/syscall.h"
#include <stdint.h>
uint32_t __do_syscall( uint32_t number, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f )
{
    uint32_t ret;
    asm volatile (
        "                   \
        push %%ebp;           \
        mov %7, %%ebp;           \
        int $0x80;            \
        pop %%ebp;            \
        mov %%eax, %0;      \
        "                  
        : "=g"(ret) : "b"(a), "c"(b), "d"(c), "S"(d), "D"(e), "a"(number), "g"(f)
    );
    return ret;
}

int read( int fd, void* buf, size_t count )
{
    return __do_syscall( SYSNO(read), fd, (uint32_t)buf, (uint32_t)count, 0,0,0 );
}
int write( int fd, const void* buf, size_t count )
{
    return __do_syscall( SYSNO(write), fd, (uint32_t)buf, (uint32_t)count, 0, 0, 0 );
}
int getcwd(char* buf, size_t len)
{
    return __do_syscall( SYSNO(getcwd), buf, len,0,0,0,0 );
}
int chdir( const char* path )
{
    return __do_syscall( SYSNO(chdir),path,0,0,0,0,0 );
}
int fchdir( int fd )
{
    return __do_syscall( SYSNO(fchdir), fd, 0,0,0,0,0 );
}
int fork()
{
    return __do_syscall( SYSNO(fork), 0,0,0,0,0,0 );
}
int execve( const char* filename, const char** argv, const char* envp )
{
    return __do_syscall( SYSNO(execve), filename, argv, envp, 0, 0, 0 );
}
int stat( const char* filename, struct fstat* buf )
{
    return __do_syscall( SYSNO(stat), filename, (uint32_t)buf, 0,0,0,0 );
}
int fstat( int fd, struct fstat* buf )
{
    return __do_syscall( SYSNO(fstat), fd, (uint32_t)buf, 0,0,0,0 );
}
int lstat( const char* filename, struct fstat* buf )
{
    return __do_syscall( SYSNO(lstat), (uint32_t)filename, (uint32_t)buf, 0,0,0,0 );
}
