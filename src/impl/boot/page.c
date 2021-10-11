#include "boot/page.h"
#include "stdlib/assert.h"
#include "stdlib/kmalloc.h"
#include "stdlib/string.h"
//https://wiki.osdev.org/Paging

extern page_dir_t boot_page_directory;

phys_addr phys_addr_of( const void* virtual_addr )
{
    uint32_t directory_idx = ((uint32_t)virtual_addr) >> 22;
    uint32_t table_idx = ((uint32_t)virtual_addr) >> 12 & 0x03FF;
    page_table_t* table = (page_table_t*) boot_page_directory.virtuals[directory_idx];
    return table->pages[table_idx].addr * PAGE_SIZE;
}

#pragma pack(1)
typedef struct va_conv
{
    uint32_t off: 12;
    uint32_t ti:  10;
    uint32_t di:  10;
} va_conv_t;
#pragma pack(0)


void wire_page( phys_addr phys, const void* virt, page_table_ent_t flags )
{
    va_conv_t converter;
    *(uint32_t*)(&converter) = (uint32_t) virt;
    uint32_t directory_idx = converter.di;
    uint32_t table_idx = converter.ti;

    page_table_t* table = (page_table_t*) boot_page_directory.virtuals[directory_idx];
    if (!table)
    {
        phys_addr tphys;
        table = kmalloc_page_struct((phys_addr*)(&tphys));
        memset(table, 0, PAGE_SIZE);
        boot_page_directory.tables[directory_idx].frame = tphys >> 12;
        boot_page_directory.tables[directory_idx].present = 1;
        boot_page_directory.tables[directory_idx].rw = 1;
        boot_page_directory.virtuals[directory_idx] = table;
    }
    page_table_ent_t* te = &table->pages[table_idx];

    *te = flags;
    te->addr = phys >> 12;
    te->present = 1;
    te->rw = 1;
    __invlpg_flush();
}

const void* next_virt()
{
    //https://compas.cs.stonybrook.edu/~nhonarmand/courses/fa17/cse306/slides/07-x86_vm.pdf
    return NULL;
}