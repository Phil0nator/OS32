#include "os32std.h"
#include "os32io.h"

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

int _start(int argc, char const *argv[])
{
    // TODO
    // Change ELF loader to not re-allocate pages
    // for programs calling execve that already have
    // usermode address-space allocated

    // Maybe that will fix the page fault?

    puts("ls: run\n");
    for(;;);
}