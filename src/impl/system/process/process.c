#include "system/process/process.h"
#include "stdlib/kmalloc.h"
#include "stdlib/string.h"
#include "drivers/cr0.h"
#include "system/filesystems/vfs.h"
#include "boot/page.h"


void process_create( process_t* dest )
{
    dest->pdir = mkpd( NULL );
    dest->next = NULL;
    dest->eip = 0;
    dest->esp = 0;
    dest->ebp = 0;
    vfs_setup_proc(dest);
    memset(dest->wd, 0, sizeof(dest->wd));
    dest->wd[0]='/';
}
void process_destroy( process_t* proc )
{
    kfree(proc->pdir);
    vfs_clean_proc(proc);

}
void process_start( process_t* proc, void (*entrypoint)() )
{
    set_pd( proc->pdir );

    kmalloc_alloc_pages(current_page_directory, STACK_SIZE/PAGE_SIZE, KERNEL_STACK_START, (page_table_ent_t){.present=1, .rw=1} );
    kmalloc_alloc_pages(current_page_directory, STACK_SIZE/PAGE_SIZE, USER_STACK_START, (page_table_ent_t){.present=1, .rw=1, .user=1});
    proc->eip = (uint32_t) entrypoint;
    proc->esp = USER_STACK_START+STACK_SIZE-32;
    proc->ebp = USER_STACK_START;
    

    tss_enter_usermode( entrypoint, USER_STACK_START + STACK_SIZE-32 );
}