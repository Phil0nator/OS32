#include "os32std.h"
#include "os32io.h"

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

int _start(int argc, char const *argv[])
{
    char cwd[PATH_MAX];
    char cmd[1024] = {0};
    while(true)
    {
        getcwd(cwd, PATH_MAX);
        puts("root@os32:");
        puts(cwd);
        puts("$");
        
        int len = read( STDIN_FILENO, cmd, 1024 );
        if ( len <= 0 ) continue;
        cmd[len-1] = 0;
        // puts(cmd);
        char cmd_start [32] = {0};
        size_t space = strchr( cmd, ' ');
        if (space == -1UL) space = strlen(cmd);
        memcpy(cmd_start, cmd, space);        

        if (strequ(cmd_start, "cd"))
        {
            struct fstat fcheck;
            int err = stat( cmd + space + 1, &fcheck );
            if (err == OS32_SUCCESS)
            {
                chdir( cmd + space + 1 );
            }
            else
            {
                puts("cd: ");
                puts(strerror( err ));
                putc('\n');
            }
        }
        else
        {
            // char binpath[PATH_MAX] = "/initrd/bin/";
            // strcpy(binpath+12, cmd_start);
            // int pid = fork();
            // if (!pid)
            // {
            //     execve( binpath, NULL, NULL );
            // }
            // else
            // {
            //     for(;;);
            // }
        }

        bzero( cmd, 1024 );

    }
    return 0;
}
