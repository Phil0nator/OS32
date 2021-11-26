#include "os32std.h"
int bss;
int data = 1;
const char* str = "fancy string";
int _start(int argc, char const *argv[])
{
    write( 1, "Hello World!", 12 );
    for(;;);   
    return argc;
}
