#ifndef OS32_PROCESS
#define OS32_PROCESS
#include <stdint.h>
#include <stdbool.h>
#include "boot/tss.h"
#include "boot/page.h"

typedef __pid_t pid_t;
typedef int     tid_t;

typedef struct process_head
{
    pid_t pid;
    tid_t tid;
    tss_t tss;

    page_dir_t pdir;
    

} process_head_t;

#endif