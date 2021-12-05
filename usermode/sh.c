#include "os32std.h"
#include "os32io.h"

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

int _start(int argc, char const *argv[])
{
    char cwd[PATH_MAX];
    char cmd[1024] = {0};
    getcwd(cwd, PATH_MAX);
    while(true)
    {
        puts("root@os32:");
        puts(cwd);
        puts("$");
        
        read( STDIN_FILENO, cmd, 1024 );
        // puts(cmd);
        char cmd_start [32] = {0};
        size_t space = strchr( cmd, ' ');
        if (space == -1UL) space = strlen(cmd);
        memcpy(cmd_start, cmd, space);        

        if (strequ(cmd_start, "cd"))
        {
            
        }
        else
        {

        }

        bzero( cmd, 1024 );

    }
    return 0;
}
