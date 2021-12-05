#include "syscall_registry.h"
#include "system/syscalls/syscall.h"
#include "impl/impl.h"
#define REGISTER_SYSCALL(name) [ SYSNO(name) ] = (syscall_routine_t) __s_##name

syscall_routine_t __os32_syscall_registry[256] = 
{
    REGISTER_SYSCALL(read),
    REGISTER_SYSCALL(write),
    REGISTER_SYSCALL(getcwd),
    REGISTER_SYSCALL(chdir),
    REGISTER_SYSCALL(fchdir),
    REGISTER_SYSCALL(fork),
    REGISTER_SYSCALL(execve)
};