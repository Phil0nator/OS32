#include "system/sync/spinlock.h"
#include <stddef.h>
#include "system/sync/intrin.h"
#include "system/process/multitasking.h"

void aquire_spinlock( spinlock_t* s )
{
    while (!cmpxchg( &s->mtx, 0, 1 ))
    {
        __yield();
        __pause();
    }
}
void release_spinlock( spinlock_t* s )
{
    s->mtx = 0;
}
