#ifndef OS32_MULTITASKING
#define OS32_MULTITASKING
#include <stddef.h>
#include "process.h"

#define OS32_MAX_ZOMBIE 1024

extern volatile process_t* current_process;
extern volatile process_t* process_list;


typedef void (*multitasking_newproc_subroutine)(pid_t pid);
typedef int priority_t;

#define PRI_MIN 1
#define PRI_LOW 2
#define PRI_MED 4
#define PRI_HIGH 8
#define PRI_MAX 16


multitasking_newproc_subroutine 
register_on_newproc_subroutine(multitasking_newproc_subroutine newsub);


err_t __install_multitasking();

int __fork();
void __procswitch();
void move_stack(void* destination, size_t size);
int __getpid();
void __yield();
err_t __setpriority( priority_t pri );

int __spawn(const char* path, const char** argv, const char* envp );
err_t __exec( const char* path,  const char** argv, const char* envp  );
void __exit(int status);
void clean_processes();

process_t* get_proc_by_id( pid_t pid );

zombie_t pull_zombie_by_id( pid_t pid, bool await );
zombie_t pull_zombie_by_parent( pid_t pid, bool await );


#endif