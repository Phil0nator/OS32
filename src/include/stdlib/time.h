#ifndef OS32_TIMEH
#define OS32_TIMEH
#include <stdint.h>

typedef uint32_t pxtime_t;
pxtime_t time(pxtime_t* dest);

#endif