#ifndef OS32_PROCESS
#define OS32_PROCESS
#include <stdint.h>
#include <stdbool.h>
#include "boot/tss.h"
#include "boot/page.h"

typedef int pid_t;
typedef int tid_t;

typedef struct process
{
    pid_t pid;
    tid_t tid;
    tss_t tss;

    page_dir_t* pdir;
    phys_addr pdir_phys;

} process_t;

void process_create( process_t* dest );
void process_destroy( process_t* proc );
void process_start( process_t* proc, void (*entrypoint)() );

#endif