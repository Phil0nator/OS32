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

#define DEFAULT_QUANTUM 0x2


typedef int pid_t;
typedef int tid_t;

// Structure represent a process
typedef struct process
{
    // process id
    pid_t pid;
    tid_t tid;

    // page directory
    page_dir_t* pdir;
    // stack, base, and instruction pointers
    uint32_t esp, ebp, eip;
    // file descriptor table
    fd_t local_fdt[128];
    // current working directory
    char wd[VFS_MAX_PATH];
    // user id
    uid_t uid;
    // status info
    int status;
    // scheduler quantum info
    size_t quantum;
    size_t quantum_progress;

    // parent process's pid
    pid_t parent;
    // next for singly linked list
    struct process* next;

} process_t;

// struct for zombie process info
typedef struct zombie
{
    // pid of process
    pid_t pid;
    // pid of parent process
    pid_t parent;
    // exit status 
    int status;
} zombie_t;

/**
 * @brief create a process
 * 
 * @param dest scribble dest
 * @param io (1 = create new fd's), (0 = do not populate fdt)
 */
void process_create( process_t* dest, bool io );
/**
 * @brief free resources used by a process
 * 
 * (WARNING) does not free the given 'proc' pointer itself
 * 
 * @param proc source
 */
void process_destroy( process_t* proc );

/**
 * @brief start a process in usermode
 * 
 * This function should NEVER return
 * 
 * @param proc populated process
 * @param entrypoint entrypoint of process
 * @param argv arguments to give process (NULL terminated)
 * @param envp environment variables
 */
void process_start( process_t* proc, void (*entrypoint)(), const char** argv, const char* envp  );

#endif