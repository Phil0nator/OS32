#include "os32io.h"
int bss;
int data = 1;
const char* str = "fancy string";
int _start(int argc, char const *argv[])
{
    puts("Hello World!\n");
    _exit(0);
}
