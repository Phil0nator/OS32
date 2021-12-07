#ifndef OS32_SYS_SYSCALL
#define OS32_SYS_SYSCALL
#include "sys/types.h"
#include "system/syscalls/syscall.h"

uint32_t __do_syscall( uint32_t number, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f );
int errno_wrap( int ret );

#endif