#ifndef OS32_TIMEH
#define OS32_TIMEH
#include <stdint.h>

// Unix time
typedef uint32_t pxtime_t;
// Get current unix time
pxtime_t time(pxtime_t* dest);

#endif