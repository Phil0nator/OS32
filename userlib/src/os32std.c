#include "os32std.h"
#include "system/syscalls/syscall.h"
#include <stdint.h>
uint32_t __do_syscall( uint32_t number, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f )
{
    asm volatile (
        "                   \
        push %%ebp;           \
        push %6;            \
        push %5;            \
        push %0;            \
        push %1;            \
        push %2;            \
        push %3;            \
        push %4;            \
        pop %%edi;          \
        pop %%esi;          \
        pop %%edx;          \
        pop %%ecx;          \
        pop %%ebx;          \
        pop %%ebp;          \
        pop %%eax;          \
        int $0x80;            \
        pop %%ebp;            \
        "                  
        : : "b"(a), "c"(b), "d"(c), "S"(d), "D"(e), "a"(f), "g"(number)
    );
}

int read( int fd, void* buf, size_t count );
int write( int fd, void* buf, size_t count )
{
    __do_syscall( SYSNO(write), fd, (uint32_t)buf, (uint32_t)count, 0, 0, 0 );
}