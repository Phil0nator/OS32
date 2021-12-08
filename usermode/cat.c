#include "os32std.h"
#include "os32io.h"
#include "sys/dirent.h"

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

int _start( int argc, const char** argv, const char* envp )
{
    for (size_t i = 0; i < argc; i++)
    {
        putc('\t');
        // puts(argv[i]);
        putc('\n');
    }
    _exit(0);
}