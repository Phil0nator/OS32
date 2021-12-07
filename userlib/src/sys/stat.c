#include "sys/stat.h"
#include "syscall.h"

int stat( const char* filename, struct fstat* buf )
{
    return errno_wrap( __do_syscall( SYSNO(stat), filename, (uint32_t)buf, 0,0,0,0 ));
}
int fstat( int fd, struct fstat* buf )
{
    return errno_wrap( __do_syscall( SYSNO(fstat), fd, (uint32_t)buf, 0,0,0,0 ));
}
int lstat( const char* filename, struct fstat* buf )
{
    return errno_wrap( __do_syscall( SYSNO(lstat), (uint32_t)filename, (uint32_t)buf, 0,0,0,0 ));
}
