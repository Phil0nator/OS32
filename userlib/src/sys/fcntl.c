#include "sys/fcntl.h"
#include "syscall.h"

int open(const char *pathname, int flags, mode_t mode)
{
    return errno_wrap( __do_syscall( pathname, flags, mode, 0, 0, 0, 0 ) );
}

int creat(const char *pathname, mode_t mode)
{

}
