#include "system/sync/spinlock.h"
#include <stddef.h>
#include "system/sync/intrin.h"

void aquire_spinlock( spinlock_t* s )
{
    while (!cmpxchg( &s->mtx, 0, 1 ))
    {
        __pause();
    }
}
void release_spinlocK( spinlock_t* s )
{
    s->mtx = 0;
}
