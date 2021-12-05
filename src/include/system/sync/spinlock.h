#ifndef OS32_SPINLOCK
#define OS32_SPINLOCK

typedef struct spinlock
{
    volatile int mtx;
} spinlock_t;

void aquire_spinlock( spinlock_t* s );
void release_spinlock( spinlock_t* s );

#endif