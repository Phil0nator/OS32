#include "os32std.h"
#include "os32io.h"

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

int _start(int argc, char const *argv[])
{
    while(true)
    {
        puts("root@os32:/$");
        for(;;);
    }
    return 0;
}
