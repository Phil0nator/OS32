#include "system/process/process.h"
#include "stdlib/kmalloc.h"
#include "stdlib/string.h"
#include "drivers/cr0.h"
#include "system/filesystems/vfs.h"
#include "boot/page.h"
#include "stdlib/instructions.h"


void process_create( process_t* dest, bool io )
{
    dest->pdir = mkpd( NULL );
    dest->next = NULL;
    dest->eip = 0;
    dest->esp = 0;
    dest->ebp = 0;
    dest->quantum = DEFAULT_QUANTUM;
    dest->quantum_progress;
    dest->status = 0;
    if (io) vfs_setup_proc(dest);
    memset(dest->wd, 0, sizeof(dest->wd));
    dest->wd[0]='/';
}
void process_destroy( process_t* proc )
{
    kfree(proc->pdir);
    if (proc->parent == -1 ) vfs_clean_proc(proc);

}
void process_start( process_t* proc, void (*entrypoint)(), const char** argv, const char* envp  )
{
    set_pd( proc->pdir );

    uint32_t sp;
    __get_esp(sp);

    if (!( sp >= KERNEL_STACK_START )) kmalloc_alloc_pages(current_page_directory, STACK_SIZE/PAGE_SIZE, KERNEL_STACK_START, (page_table_ent_t){.present=1, .rw=1} );
    if ( !phys_addr_of(current_page_directory, USER_STACK_START) )  kmalloc_alloc_pages(current_page_directory, STACK_SIZE/PAGE_SIZE, USER_STACK_START, (page_table_ent_t){.present=1, .rw=1, .user=1});
    proc->eip = (uint32_t) entrypoint;
    proc->esp = USER_STACK_START+STACK_SIZE-32;
    proc->ebp = USER_STACK_START;
    
    int argc = 0;
    if(argv)
    {
        while(*argv)
        {
            size_t len = strlen(*argv);
            memcpy( proc->esp-len, *argv, len );
            proc->esp-=len;
            *argv = proc->esp;

            argv++;
            argc++;
        }
    }
    memcpy( proc->esp-(argc*4), argv, argc*4 );
    proc->esp -= argc*4;
    argv = proc->esp;

    tss_enter_usermode( entrypoint, proc->esp, argc, argv, envp );
}