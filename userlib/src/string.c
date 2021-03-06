#include "string.h"

void memcpy( void * dest, const void * source, size_t bytes )
{
    for (size_t i = 0;i < bytes;i++)
        ((char*)dest)[i]=((const char*)source)[i];
}
void memset( void* dest, char byte, size_t count )
{
    for(;count;count--){
        ((char*)dest)[count-1] = byte;
    }
}


void bzero( void* dest, size_t bytes )
{
    for (;bytes;bytes--) ((char*)dest)[bytes-1] = 0;
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
size_t strchr( const char* str, char chr )
{
    const char* ogstr = str;
    while (*str && *str != chr) str++;
    return *str == chr ? str-ogstr : -1; 
}
bool strequ( const char* a, const char* b )
{
    while (true)
    {
        if (*a != *b)
        {
            return false;
        }
        a++;
        b++;
        if (*a == *b && *a == 0) break;
    }
    return true;
}
bool starts_with( const char* a, const char* b )
{
    while (*b)
    {
        if (*a != *b)
        {
            return false;
        }
        a++;
        b++;
    }
    return true;
}