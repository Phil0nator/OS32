#ifndef OS32_SYNC_INTRIN
#define OS32_SYNC_INTRIN

#define __cmpxchg(x, val, newval, eqdest) asm volatile ( "\
lock cmpxchg %%ecx, (%%ebx);\
setz %0;\
" : "=g"(eqdest) : "a"(val), "b"(x), "c"(newval) )

#define __pause(x) asm volatile ("pause")

int cmpxchg( volatile int* x, int val, int newval );

#endif