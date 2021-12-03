#include "os32std.h"
#include "system/syscalls/syscall.h"
#include <stdint.h>
uint32_t __do_syscall( uint32_t number, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f )
{
    asm volatile (
        "                   \
        push %%ebp;           \
        mov %6, %%ebp;           \
        int $0x80;            \
        pop %%ebp;            \
        leave;              \
        ret;                \
        "                  
        : : "b"(a), "c"(b), "d"(c), "S"(d), "D"(e), "a"(number), "g"(f)
    );
}

int read( int fd, void* buf, size_t count );
int write( int fd, const void* buf, size_t count )
{
    __do_syscall( SYSNO(write), fd, (uint32_t)buf, (uint32_t)count, 0, 0, 0 );
}