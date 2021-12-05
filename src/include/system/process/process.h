#ifndef OS32_PROCESS
#define OS32_PROCESS
#include <stdint.h>
#include <stdbool.h>
#include "boot/tss.h"
#include "boot/page.h"
#include "system/filesystems/vfs.h"

#define KERNEL_STACK_START ((void*)0xf0000000)
#define USER_STACK_START ((void*)0xe0000000)
#define STACK_SIZE (PAGE_SIZE*16)



typedef int pid_t;
typedef int tid_t;

typedef struct process
{
    pid_t pid;
    tid_t tid;

    page_dir_t* pdir;
    uint32_t esp, ebp, eip;
    fd_t local_fdt[128];
    char wd[VFS_MAX_PATH];
    uid_t uid;
    int status;

    pid_t parent;
    struct process* next;

} process_t;


void process_create( process_t* dest );
void process_destroy( process_t* proc );
void process_start( process_t* proc, void (*entrypoint)(), const char* argv, const char* envp  );

#endif