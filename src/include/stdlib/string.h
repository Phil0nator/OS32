#ifndef OS32_STRING
#define OS32_STRING
#include <stddef.h>

void memcpy( void * dest, const void * source, size_t bytes );
void memset( void * dest, char byte, size_t count );
void memmove( void* dest, const void* source, size_t bytes );
void bzero( char* dest, size_t bytes );

int memcmp( const char* a, const char* b, size_t bytes );
int memequ( const char* a, const char* b, size_t bytes );

size_t strcpy( char* dest, const char* src );
size_t strncpy( char* dest, size_t n, const char* src );
size_t strlen( const char* data );

#endif