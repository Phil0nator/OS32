#include "../src/include/system/syscalls/syscall.h"
int bss;
int data = 1;
const char* str = "fancy string";
int _start(int argc, char const *argv[])
{
    __syscall(SYSNO(read)); 
    for(;;);   
    return argc;
}
