#ifndef OS32_SPINLOCK
#define OS32_SPINLOCK

// basic spinlock impl
typedef struct spinlock
{
    volatile int mtx;
} spinlock_t;

/**
 * @brief aquire
 * 
 * @param s 
 */
void aquire_spinlock( spinlock_t* s );
/**
 * @brief release
 * 
 * @param s 
 */
void release_spinlock( spinlock_t* s );

#endif