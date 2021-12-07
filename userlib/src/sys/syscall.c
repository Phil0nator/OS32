#include "syscall.h"
extern int __errno;
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
int errno_wrap( int ret )
{
    if (ret < 0)
    {
        __errno = -ret;
        ret = -1;
    }
    return ret;
}