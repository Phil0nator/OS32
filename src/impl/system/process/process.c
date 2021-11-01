#include "system/process/process.h"
#include "stdlib/kmalloc.h"

#define KERNEL_STACK_START ((void*)0xf0000000)
#define USER_STACK_START ((void*)0xe0000000)
#define STACK_SIZE (PAGE_SIZE*16)

void process_create( process_t* dest )
{

    dest->pdir = kmalloc_page_struct(&dest->pdir_phys);
}
void process_destroy( process_t* proc )
{
    kfree(proc->pdir);
}
void process_start( process_t* proc, void (*entrypoint)() )
{
    proc->tss.cr3 = proc->pdir_phys;
    proc->tss.ss0 = 0x10;

    
    
    kmalloc_alloc_pages(proc->pdir, STACK_SIZE/PAGE_SIZE, KERNEL_STACK_START, (page_table_ent_t){.present=1, .rw=1} );
    kmalloc_alloc_pages(proc->pdir, STACK_SIZE/PAGE_SIZE, USER_STACK_START, (page_table_ent_t){.present=1, .rw=1, .user=1});
    proc->tss.esp0 = (uint32_t)KERNEL_STACK_START + STACK_SIZE;
    proc->tss.esp2 = (uint32_t)KERNEL_STACK_START + STACK_SIZE;
    tss_enter_usermode( entrypoint, &proc->tss );
}