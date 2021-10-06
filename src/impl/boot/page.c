#include "boot/page.h"
#include "stdlib/assert.h"
//https://wiki.osdev.org/Paging

extern page_dir_t boot_page_directory;

phys_addr phys_addr_of( const void* virtual_addr )
{
    uint32_t directory_idx = ((uint32_t)virtual_addr) >> 22;
    uint32_t table_idx = ((uint32_t)virtual_addr) >> 12 & 0x03FF;

    
}
void wire_page( phys_addr phys, const void* virt, page_table_ent_t flags )
{
    uint32_t directory_idx = ((uint32_t)virt) >> 22;
    uint32_t table_idx = ((uint32_t)virt) >> 12 & 0x03FF;

    page_table_t* table = (page_table_t*) boot_page_directory.tables[directory_idx].frame;
    page_table_ent_t* te = &table->pages[table_idx];

    *te = flags;
    te->addr = phys;
    te->present = 1;
}

const void* next_virt()
{
    //https://compas.cs.stonybrook.edu/~nhonarmand/courses/fa17/cse306/slides/07-x86_vm.pdf
    
}