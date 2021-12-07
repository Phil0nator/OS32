#include "./syscall.h"
#include "sys/wait.h"

pid_t wait(int *stat_loc)
{
    return waitpid( -1, stat_loc, 0 );
}
pid_t waitpid(pid_t pid, int *stat_loc, int options)
{
    return errno_wrap( __do_syscall( SYSNO(wait4), (uint32_t)pid, (uint32_t)stat_loc, (uint32_t)options, NULL, 0, 0 ));
}
