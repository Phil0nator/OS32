#include "os32io.h"
#include "os32std.h"
#include "string.h"

void puts( const char* str )
{
    write( STDOUT_FILENO, str, strlen(str) );
}

void putc( char c )
{
    write( STDOUT_FILENO, &c, 1 );
}