#ifndef OS32_STRING
#define OS32_STRING
#include <stddef.h>

void memcpy( void * dest, const void * source, size_t bytes );
void memset( void * dest, char byte, size_t count );
void memmove( void* dest, const void* source, size_t bytes );
#endif