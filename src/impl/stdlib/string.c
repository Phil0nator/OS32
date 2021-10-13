#include "stdlib/string.h"

void memcpy( void * dest, const void * source, size_t bytes )
{
    for (;bytes;bytes--)
        ((char*)dest)[bytes-1]=((const char*)source)[bytes-1];
}
void memset( void* dest, char byte, size_t count )
{
    for(;count;count--){
        ((char*)dest)[count-1] = byte;
    }
}


void bzero( char* dest, size_t bytes )
{
    for (;bytes;bytes--) dest[bytes-1] = 0;
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

int memcmp( const char* a, const char* b, size_t bytes )
{
    int output = 0;
    for (;bytes;bytes--) output += a[bytes-1] != b[bytes-1];
    return output;
}
int memequ( const char* a, const char* b, size_t bytes )
{
    for (size_t i = 0; i < bytes; i++)
        if ( a[i] != b[i] ) return 0;
    return 1;
}

size_t strcpy( char* dest, const char* src )
{
    char* ogd = dest;
    while ((*(dest++)=*(src++)));
    return dest-ogd;
}
size_t strncpy( char* dest, size_t n, const char* src )
{
    char* ogd = dest;
    for (;((size_t)(dest-ogd)<n) && *src;dest++,src++) *dest = *src;
    return dest-ogd;
}
size_t strlen( const char* data )
{
    const char* og = data;
    while (*(data++));
    return data-og;
}
