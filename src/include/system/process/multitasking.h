#ifndef OS32_MULTITASKING
#define OS32_MULTITASKING
#include <stddef.h>
#include "process.h"

extern volatile process_t* current_process;
extern volatile process_t* process_list;


err_t __install_multitasking();

int __fork();
void __procswitch();
void move_stack(void* destination, size_t size);
int __getpid();


int __spawn(const char* path);
process_t* get_proc_by_id( pid_t pid );


#endif