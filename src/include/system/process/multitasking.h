#ifndef OS32_MULTITASKING
#define OS32_MULTITASKING
#include <stddef.h>
#include "process.h"

extern volatile process_t* current_process;
extern volatile process_t* process_list;

typedef void (*multitasking_newproc_subroutine)(pid_t pid);


multitasking_newproc_subroutine 
register_on_newproc_subroutine(multitasking_newproc_subroutine newsub);


err_t __install_multitasking();

int __fork();
void __procswitch();
void move_stack(void* destination, size_t size);
int __getpid();


int __spawn(const char* path, const char** argv, const char* envp );
err_t __exec( const char* path,  const char** argv, const char* envp  );
process_t* get_proc_by_id( pid_t pid );


#endif