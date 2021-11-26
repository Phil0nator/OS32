#include "os32std.h"
#include "system/syscalls/syscall.h"
#include <stdint.h>
uint32_t __do_syscall( uint32_t number, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f )
{
    asm volatile (
        "                   \
        push %%ebp;           \
        push %6;              \
        mov %0, %%ebx;      \
        mov %1, %%ecx;      \
        mov %2, %%edx;      \
        mov %3, %%esi;      \
        mov %4, %%edi;      \
        mov %5, %%ebp;      \
        pop %%eax;          \
        int $0x80;            \
        pop %%ebp;            \
        ret;                \
        "                  
        : : "r"(a), "r"(b), "r"(c), "r"(d), "r"(e), "r"(f), ""(number)
    );
}

int read( int fd, void* buf, size_t count );
int write( int fd, void* buf, size_t count )
{
    __do_syscall( SYSNO(write), fd, (uint32_t)buf, (uint32_t)count, 0, 0, 0 );
}