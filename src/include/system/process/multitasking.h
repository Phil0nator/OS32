#ifndef OS32_MULTITASKING
#define OS32_MULTITASKING
#include <stddef.h>
#include "process.h"

// max zombie structs to hold at once
#define OS32_MAX_ZOMBIE 1024

// always up to date current process pointer
extern volatile process_t* current_process;

// all processes (single linked list)
extern volatile process_t* process_list;

// depricated
typedef void (*multitasking_newproc_subroutine)(pid_t pid);

// process priority
typedef int priority_t;

// process priority quantum counts
#define PRI_MIN 1
#define PRI_LOW 2
#define PRI_MED 4
#define PRI_HIGH 8
#define PRI_MAX 16

// depricated
multitasking_newproc_subroutine 
register_on_newproc_subroutine(multitasking_newproc_subroutine newsub);

// initial setup
err_t __install_multitasking();


/**
 * @brief 
 * UNIX-like fork
 * Creates a new process with a copied address-space.
 * @return int either the child's pid, or 0 if this is the child process.
 */
int __fork();

/**
 * @brief 
 * Used by the PIT to switch to the next process
 * (not to be called otherwise, to give up a timeslice use __yield)
 */
void __procswitch();

/**
 * @brief move the current stack to a new virtual address
 * (might overwrite certain values on the stack)
 * @param destination new virtual address
 * @param size size of the current stack
 */
void move_stack(void* destination, size_t size);

/**
 * @brief get the current process's pid
 * @return int the current process's pid
 */
int __getpid();

/**
 * @brief give up the current process's timeslice.
 * this function will not return until at least the next timeslice
 */
void __yield();

/**
 * @brief set the priority of the current process
 * (quantum value 1-16) use macros PRI_ ...
 * 
 * @param pri quantum priority
 * @return err_t might return error for invalid values of pri (use macros to prevent this)
 */
err_t __setpriority( priority_t pri );


/**
 * @brief Spawn a new process from an executable file in usermode.
 * 
 * __spawn is actually just a helpful combination of __fork() and __exec()
 * 
 * @param path path to executable (vfs)
 * @param argv argv to pass to the process (NULL terminated array)
 * @param envp enironment variables to pass to the process
 * @return int the child process's pid
 */
int __spawn(const char* path, const char** argv, const char* envp );

/**
 * @brief execute an executable file in usermode
 * 
 * 
 * @param path path to executable (vfs)
 * @param argv argv to pass to the process (NULL terminated array)
 * @param envp environment variables to pass to the process
 * @return err_t on success, this function does not return. 
 *  Otherwise, error is returned and errno is set.
 */
err_t __exec( const char* path,  const char** argv, const char* envp  );

/**
 * @brief exit the current process
 * (does not return under any circumstances)
 * @param status exit status
 */
void __exit(int status);

/**
 * @brief called by the kernel thread to delete old processes
 * and update the zombie set. Not to be called otherwise
 */
void clean_processes();

/**
 * @brief Get a proc by pid
 * 
 * @param pid process identifier
 * @return process_t* either the requested process, or fail
 */
process_t* get_proc_by_id( pid_t pid );

/**
 * @brief pull a zombie struct by it's pid
 * 
 * @param pid zombie pid
 * @param await if true, blocking, otherwise it will return immediately
 * @return zombie_t either the zombie requested, or a (pid=-1) zombie if 
 * the requested zombie could not be found
 */
zombie_t pull_zombie_by_id( pid_t pid, bool await );

/**
 * @brief pull a zombie struct by a parent pid
 * This will return the first zombie with a parent equal to (pid)
 * @param pid parent pid
 * @param await if true, blocking, otherwise it will return immediately
 * @return zombie_t either the zombie requested, or a (pid=-1) zombie if 
 * the requested zombie could not be found
 */
zombie_t pull_zombie_by_parent( pid_t pid, bool await );


#endif