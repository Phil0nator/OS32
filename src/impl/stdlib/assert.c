#include "stdlib/assert.h"
#include "boot/vga.h"

void __assert_failed_vga( 
    const char* expr, 
    const char* file, 
    size_t line 
)
{
    vgaPrintf("%- assertion \"%s\" failed at %s:%i.\n", expr, file, line);
    for(;;);
}