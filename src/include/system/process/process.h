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

    page_dir_t pdir;
    

} process_t;

#endif