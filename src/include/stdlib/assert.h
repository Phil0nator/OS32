#ifndef OS32_ASSERT
#define OS32_ASSERT
#include <stddef.h>

#define vga_assert( condition ) \
    if (!(condition)) \
        {__assert_failed_vga( #condition, __FILE__, __LINE__ );}

void __assert_failed_vga( 
    const char* expr, 
    const char* file, 
    size_t line 
);

#endif