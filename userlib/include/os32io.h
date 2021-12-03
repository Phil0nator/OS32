#ifndef OS32_STDIO
#define OS32_STDIO

#define STDOUT_FILENO 1
#define STDERR_FILENO 2
#define STDIN_FILENO 0


void puts( const char* str );

void putc( char c );

void printf( const char* format, ... );



#endif