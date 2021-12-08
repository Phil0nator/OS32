#include "system/process/multitasking.h"
#include "boot/page.h"
#include "system/process/process.h"
#include "stdlib/instructions.h"
#include "stdlib/kmalloc.h"
#include "system/filesystems/vfs.h"
#include "stdlib/string.h"
#include "system/process/elf.h"
#include "drivers/timer.h"
#include "stdlib/static_set.h"
// https://web.archive.org/web/20160326122214/http://jamesmolloy.co.uk/tutorial_html/9.-Multitasking.html

// Used to return from a task switch
#define DUMMY_SWITCH 0x123


volatile process_t* current_process;

// singly linked list
volatile process_t* process_list;

// static set (see static_set.h) of zombie process info
statset_decl(zombie_t, zombies, OS32_MAX_ZOMBIE);
// list of pid's of processes that need to be cleaned up by the kernel
statset_decl( pid_t, deadprocs, OS32_MAX_ZOMBIE );

// pid counter
pid_t next_pid = 1;

// Depricated
multitasking_newproc_subroutine on_new_sub = NULL;

multitasking_newproc_subroutine 
register_on_newproc_subroutine(multitasking_newproc_subroutine newsub)
{
    multitasking_newproc_subroutine old = on_new_sub;
    on_new_sub = newsub;
    return old;
}

// defined in boot.asm, the original stack top
extern void* __bootstack_top;


void move_stack( void* dest, size_t size )
{
    // Allocate sufficient pages at the new address
    kmalloc_alloc_pages( current_page_directory, size/PAGE_SIZE, dest-size, (page_table_ent_t){ .present=1, .rw=1, .user=0 } );
    
    // collect relevant stack info
    uint32_t old_esp, old_ebp;
    uint32_t old_top = (uint32_t)&__bootstack_top;
    __get_esp(old_esp);
    __get_ebp(old_ebp);

    // determine the distance between the old and new stacks
    uint32_t offset = dest-old_top;
    uint32_t new_esp, new_ebp;

    // use the distance to determine the new stack and base pointers
    new_esp = old_esp + offset;
    new_ebp = old_ebp + offset;

    // copy all stack data
    memcpy( new_esp, old_esp, old_top - old_esp  );

    // update stack frame data using offset
    for (uint32_t* i = (uint32_t*)((uint32_t)dest-4); i > ((uint32_t)new_esp); i--)
    {
        if ( *i > old_esp && *i < old_top )
        {
            *i += offset;
        }
    }

    // update stack and base pointers
    __set_esp(new_esp);
    __set_ebp(new_ebp);
}

err_t __install_multitasking()
{
    __cli
    // create the first process
    current_process = (process_list = kmalloc( sizeof(process_t) ));
    process_create(current_process, true);
    current_process->pid = next_pid++;
    current_process->eip = __get_eip();
    uint32_t esp, ebp;
    __get_esp(esp);
    __get_ebp(ebp);
    current_process->esp = esp;
    current_process->ebp = ebp;
    current_process->parent = -1;
    
    // update page directory
    set_pd(current_process->pdir);   

    // move stack to standard kernel stack location 
    move_stack( 0xf0000000, 16384 );
    

    __sti
    return OS32_SUCCESS;
}

void __procswitch()
{
    // if multitasking is not yet installed, move on
    if (!current_process)
        return;
    
    // collect pointer info
    uint32_t esp, ebp, eip;
    __get_esp(esp);
    __get_ebp(ebp);

    // After this call, the value of $eax can either
    // be the instruction pointer, or the dummy value
    // depending on if this is the process we are switching to
    // or switching from.
    eip = __get_eip();
    if (eip == DUMMY_SWITCH)
    {
        // If $eax is the dummy value, we already switched tasks, we may continue as normal.
        return;
    }
    // if $eax is the actual instruction pointer, we still have to perform the task switch.
    
    // update relevant process registers
    current_process->esp = esp;
    current_process->ebp = ebp;
    current_process->eip = eip;

    // switch process virtually
    current_process = current_process->next;
    if (!current_process) current_process = process_list;    

    // load relevant process registers
    esp = current_process->esp;
    ebp = current_process->ebp;
    eip = current_process->eip;

    // tell PIC we're finished (just in case)
    outportb(0x20, 0x20);

    // virtually update page directory
    current_page_directory = current_process->pdir;

    // load stack and base poiners, load new cr3, 
    // jump to the process's given eip address,
    // load the dummy value into $eax so that the process
    // will return after we jump above ^
    asm volatile("         \
     cli;                 \
     mov %0, %%edi;       \
     mov %1, %%esp;       \
     mov %2, %%ebp;       \
     mov %3, %%cr3;       \
     mov $0x123, %%eax; \
     sti;                \
     jmp *%%edi           "
                : : "r"(eip), "r"(esp), "r"(ebp), "r"(current_process->pdir->phys));
}
static void push_proc( process_t* proc )
{
    // add proc to singly linked list
    process_t* ptr = process_list;
    if (!ptr) process_list = proc;
    while (ptr->next)
    {
        ptr = ptr->next;
    }
    ptr->next = proc;
    if (on_new_sub)
    {
        on_new_sub(proc->pid);
    }
}

int __fork_sub(volatile process_t* parent, volatile process_t* newproc, uint32_t eip)
{
    if (current_process == parent)
    {
        // if this is the parent, setup the child with relevant info
        uint32_t ebp, esp;
        __get_esp(esp);
        __get_ebp(ebp);
        newproc->ebp = ebp;
        newproc->esp = esp;
        newproc->eip = eip;
        return newproc->pid;
    }
    else
    {
        // if this is the child, simply return 0
        return 0;
    }
}

int __fork()
{
    // use eflags to push/pop interrupts state
    eflags_t eflags;
    __get_eflags(eflags);
    __cli

    // collect parent and create child
    volatile process_t* parent = current_process;
    volatile process_t* newproc = kmalloc( sizeof(process_t) );
    volatile uint32_t eip;
    process_create( newproc, false );
    
    // load relevant data
    newproc->parent = current_process->pid;
    memcpy(newproc->local_fdt, parent->local_fdt, sizeof(parent->local_fdt));
    newproc->pid = next_pid++;
    newproc->quantum = parent->quantum;
    strcpy(newproc->wd, parent->wd);

    // add new process to list
    push_proc(newproc);
    dir_dup( newproc->pdir, parent->pdir );

    // padding for later jump
    OS32_MAKEINSTR("nop;nop;nop;");
    eip = __get_eip(); 
    // use __fork_sub's stack frame to maintain data
    int ret = __fork_sub(parent, newproc, eip);
    if (ret == newproc->pid) 
    {
        // pop interrupt state
        __set_eflags(eflags);
    }
    return ret;
    
}

int __getpid()
{
    // simple getter
    return current_process->pid;
}

int __spawn(const char* path, const char** argv, const char* envp )
{
    // helper function to create new process, and load executable in the child
    pid_t pid = 0;
    if ((pid = __fork()) == 0)
    {
        __exec(path,argv,envp);
    }
    return pid;
}

err_t __exec( const char* path, const char** argv, const char* envp  )
{
    // open the file
    fd_t fd = vfs_open( path, 0, 0);
    if (fd < 0)
    {
        return OS32_ERROR;
    }
    // seek to end to determine the file's size
    vfs_seekg( fd, 0, VFS_SEEK_END );
    size_t len = vfs_tellg(fd);
    vfs_seekg( fd, 0, VFS_SEEK_SET );

    // allocate a buffer for the file
    char* data = kmalloc( len );
    if (!data || data == OS32_FAILED)
    {
        return OS32_ERROR;
    }

    // load the entire file
    err_t e = vfs_read(fd, data, len);
    if (e == OS32_ERROR)
    {
        return OS32_ERROR;
    }

    // setup an elf file structure to load as a process
    struct elf_file* elf = elf_load( data );
    if (elf == OS32_FAILED)
    {
        return OS32_ERROR;
    }
    process_t* proc = current_process;

    // use the elf structure to fully copy the elf into the address-space,
    // and get the entrypoint
    void (*entry)() = elf_load_for_exec( elf, proc );
    if (entry == OS32_FAILED)
    {
        return OS32_ERROR;
    }

    // free old data before execution
    elf_free(elf);
    kfree(data);
    vfs_close(fd);

    // enter usermode
    process_start(proc, entry, argv, envp);

    // following code SHOULD be unreachable:

    process_destroy(proc);
    proc->eip = NULL;
    for(;;);
}

process_t* get_proc_by_id( pid_t pid )
{
    // search linked list
    process_t* proc = process_list;
    if (!proc) return OS32_FAILED;
    if (proc->pid == pid) return proc;
    while (proc->next)
    {
        proc = proc->next;
        if (proc->pid == pid) return proc;
    }
    if (proc->pid == pid) return proc;
    return OS32_FAILED;

}

err_t __setpriority( priority_t pri )
{
    // assert that priority is valid
    if (pri < PRI_MIN || pri > PRI_MAX) return OS32_ERROR;

    // set the quantum
    current_process->quantum = pri;
    return OS32_SUCCESS;
}

void __yield()
{
    // yield the current process's timeslice by completing the quantum
    current_process->quantum_progress = current_process->quantum;

    // then use flags to push/pop the interrupt state
    eflags_t flags;
    __get_eflags(flags);
    __sti
    // wait one tick to ensure that this function does not return until
    // this process gets its next timeslice
    pit_waitt(1);
    __set_eflags(flags);
}

void __exit(int status)
{
    // update the status bits
    current_process->status |= status & 0xff;
    // add to dead processes
    statset_add( deadprocs, current_process->pid );
    // yield until the kernel thread cleans this process up
    while(1) __yield();
}

void clean_processes()
{
    // search the dead process list for threads to clean up
    for ( size_t i = 0; i < OS32_MAX_ZOMBIE; i++ )
    {
        // get the pid of any present items in the list
        pid_t pid;
        if (deadprocs[i].present)
        {
            pid = deadprocs[i].member;
            deadprocs[i].present = false;
        } 
        else
        {
            continue;
        }

        // search the process list for that pid
        process_t* proc = process_list;
        process_t* prev = NULL;
        while (proc->next)
        {
            if (proc->pid == pid)
            {
                break;
            }
            prev = proc;
            proc = proc->next;
        }

        // cut that process out of the list
        if (prev)
        {
            prev->next = prev->next->next;
        }
        else
        {
            process_list = process_list->next;
        }
        
        // create new zombie structure, and load relevant info
        zombie_t newzomb;
        newzomb.pid = proc->pid;
        newzomb.parent = proc->parent;
        newzomb.status = proc->status;

        // add the zombie to the zombie list
        statset_add( zombies, newzomb );

        // destroy old process info (including page directory)
        process_destroy(proc);
        // free the process structure itself
        kfree(proc);

        

    }
}


zombie_t pull_zombie_by_id( pid_t pid, bool await )
{
    __retry:
    // search the zombies for a pid
    for (size_t i = 0; i < OS32_MAX_ZOMBIE; i++)
    {
        if (zombies[i].present && zombies[i].member.pid == pid)
        {
            // pull that pid
            zombies[i].present = 0;
            return zombies[i].member;
        }
    }

    // if this call is blocking, yield and try again
    if (await)
    {
        __yield();
        goto __retry;
    }

    // otherwise return a (pid=-1) zombie
    return (zombie_t){.pid=-1};
}
zombie_t pull_zombie_by_parent( pid_t pid, bool await )
{
    // search zombies for a parent
    __retry:
    for (size_t i = 0; i < OS32_MAX_ZOMBIE; i++)
    {
        if (zombies[i].present && zombies[i].member.parent == pid)
        {
            // pull the zombie
            zombies[i].present = 0;
            return zombies[i].member;
        }
    }
    // if this call is blocking, yield and try again

    if (await)
    {
        __yield();
        goto __retry;
    }

    // otherwise return a (pid=-1) zombie
    return (zombie_t){.pid=-1};
}