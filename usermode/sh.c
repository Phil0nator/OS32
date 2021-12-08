#include "os32std.h"
#include "os32io.h"
#include "sys/stat.h"
#include "sys/wait.h"

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
        puts("poot@os32:");
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
                puts(strerror( errno ));
                putc('\n');
            }
        }
        else if (*cmd_start == 0)
        {

        }
        else
        {
            char binpath[PATH_MAX] = "/initrd/bin/";
            char* argvlist[256] = {0};
            char** argvlistptr = argvlist;
            for (char* c = cmd + space; c < cmd+sizeof(cmd); c++)
            {
                if (*c == ' ')
                {
                    *c = '\0';
                    *argvlistptr = c+1;
                    argvlistptr++;
                } 
            }
            *argvlistptr = NULL;

            strcpy(binpath+12, cmd_start);
            int pid = fork();
            if (!pid)
            {
                int err = execve( binpath, argvlist, NULL );
                puts(binpath);
                puts(": ");
                puts(strerror(errno));
                putc('\n');
                _exit(1);
            }
            else
            {
                int status;
                if (wait(&status) == OS32_ERROR)
                {
                    puts("wait: ");
                    puts(strerror(errno));
                    for(;;);
                }
            }
        }

        bzero( cmd, 1024 );

    }
    _exit(0);
}
