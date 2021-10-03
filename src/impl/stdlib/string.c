#include "stdlib/string.h"

void memcpy( void * dest, const void * source, size_t bytes )
{
    for (;bytes;bytes--)
        ((char*)dest)[bytes]=((const char*)source)[bytes];
}
void memset( void* dest, char byte, size_t count )
{
    for(;count;count--) ((char*)dest)[count] = byte;
}