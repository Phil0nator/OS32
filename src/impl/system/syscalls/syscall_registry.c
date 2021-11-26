#include "syscall_registry.h"
#include "system/syscalls/syscall.h"
#define REGISTER_SYSCALL(name) [ SYSNO(name) ] = __s_##name

syscall_routine_t __os32_syscall_registry[256] = 
{
    
};