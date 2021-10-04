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

void memmove( void* dest, const void* source, size_t bytes )
{
    if (dest < source)
    {
        for (size_t i = 0; i < bytes; i++) 
            ((char*)dest)[i] = ((const char*)source)[i];
    }
    else if (dest > source)
    {
        while (bytes--) ((char*)dest)[bytes] = ((const char*)source)[bytes];
    }
}