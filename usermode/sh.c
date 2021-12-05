#include "os32std.h"
#include "os32io.h"

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

int _start(int argc, char const *argv[])
{
    char cwd[PATH_MAX];
    getcwd(cwd, PATH_MAX);
    while(true)
    {
        puts("root@os32:");
        puts(cwd);
        puts("$");
        for(;;);
    }
    return 0;
}
