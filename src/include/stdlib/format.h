#ifndef OS32_FMT
#define OS32_FMT
#include <stddef.h>
int snprintf( char* dest, size_t n, const char* fmt, ... );
int sscanf( const char* source, const char* fmt, ... );

#endif