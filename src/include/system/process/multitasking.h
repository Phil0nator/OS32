#ifndef OS32_MULTITASKING
#define OS32_MULTITASKING
#include <stddef.h>
#include "process.h"

extern process_t* current_process;


err_t __install_multitasking();

int __fork();
void __procswitch();
void move_stack(void* destination, size_t size);
int __getpid();




#endif