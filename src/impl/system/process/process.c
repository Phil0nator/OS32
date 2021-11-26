#include "system/process/process.h"
#include "stdlib/kmalloc.h"
#include "stdlib/string.h"
#include "drivers/cr0.h"



void process_create( process_t* dest )
{
    dest->pdir = kmalloc_a(sizeof(page_dir_t), PAGE_SIZE);
    dest->pdir_phys = phys_addr_of(&boot_page_directory, dest->pdir );
    // memset(dest->pdir, 0, sizeof(page_dir_t));
    // memcpy(dest->pdir, &boot_page_directory, sizeof(boot_page_directory));
    dir_dup( dest->pdir, &boot_page_directory );
    dest->next = NULL;
    dest->eip = 0;
    dest->esp = 0;
    dest->ebp = 0;

}
void process_destroy( process_t* proc )
{
    kfree(proc->pdir);
}
void process_start( process_t* proc, void (*entrypoint)() )
{
    // proc->tss.cr3 = proc->pdir_phys;    
    // set_cr3(proc->pdir_phys);
    kmalloc_alloc_pages(&boot_page_directory, STACK_SIZE/PAGE_SIZE, KERNEL_STACK_START, (page_table_ent_t){.present=1, .rw=1} );
    kmalloc_alloc_pages(&boot_page_directory, STACK_SIZE/PAGE_SIZE, USER_STACK_START, (page_table_ent_t){.present=1, .rw=1, .user=1});
    proc->eip = (uint32_t) entrypoint;
    proc->esp = USER_STACK_START+STACK_SIZE;
    proc->ebp = USER_STACK_START;
    // memcpy(&boot_page_directory, proc->pdir, sizeof(page_dir_t));
    // __invlpg_flush();
    // uint32_t cr0;
    // cr0 = cr0_get();
    // CR0_SET_BIT(cr0, CR0_PG, 1);
    // cr0_set(cr0);
    tss_enter_usermode( entrypoint, USER_STACK_START + STACK_SIZE );
}