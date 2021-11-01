#include "boot/page.h"
#include "stdlib/assert.h"
#include "stdlib/kmalloc.h"
#include "stdlib/string.h"
//https://wiki.osdev.org/Paging

#pragma pack(1)
typedef struct va_conv
{
    uint32_t off: 12;
    uint32_t ti:  10;
    uint32_t di:  10;
} va_conv_t;
#pragma pack(0)

extern page_dir_t boot_page_directory;

phys_addr phys_addr_of(page_dir_t* page_directory, const void* virtual_addr )
{
    va_conv_t converter;
    memcpy( &converter, &virtual_addr, sizeof(virtual_addr) );
    // *(uint32_t*)(&converter) = (uint32_t) virt;
    uint32_t directory_idx = converter.di;
    uint32_t table_idx = converter.ti;
    // uint32_t directory_idx = ((uint32_t)virtual_addr) >> 22;
    // uint32_t table_idx = ((uint32_t)virtual_addr) >> 12 & 0x03FF;
    page_table_t* table = (page_table_t*) page_directory->virtuals[directory_idx];
    return table->pages[table_idx].frame * PAGE_SIZE;
}


void unwire_page( page_dir_t* page_directory, const void* virt)
{
    va_conv_t converter;
    memcpy( &converter, &virt, sizeof(virt) );
    // *(uint32_t*)(&converter) = (uint32_t) virt;
    uint32_t directory_idx = converter.di;
    uint32_t table_idx = converter.ti;
    page_table_t* table = (page_table_t*) page_directory->virtuals[directory_idx];
    if (!table) return;
    table->pages[table_idx].present = 0;
    for (size_t i = 0; i < (1024); i++)
    {
        if (table->pages[i].present) goto clean_ret;
    }
    kfree( table );
    page_directory->virtuals[directory_idx] = NULL;
    page_directory->tables[directory_idx].present = 0;

    clean_ret:
    __invlpg_flush();
}

void wire_page( page_dir_t* page_directory, phys_addr phys, const void* virt, page_table_ent_t flags )
{
    virt = (uint32_t)virt & (~0x03ff);
    va_conv_t converter;
    memcpy( &converter, &virt, sizeof(virt) );
    // *(uint32_t*)(&converter) = (uint32_t) virt;
    uint32_t directory_idx = converter.di;
    uint32_t table_idx = converter.ti;
    

    page_table_t* table = (page_table_t*) page_directory->virtuals[directory_idx];
    if (!table)
    {
        phys_addr tphys;
        table = kmalloc_page_struct((phys_addr*)(&tphys));
        memset(table, 0, PAGE_SIZE);
        page_directory->tables[directory_idx].frame = tphys >> 12 & 0x03FF;
        // *(uint32_t*)&page_directory.tables[directory_idx] = tphys;
        page_directory->tables[directory_idx].present = 1;
        page_directory->tables[directory_idx].rw = 1;
        page_directory->tables[directory_idx].user = flags.user;
        // *(uint32_t*)&page_directory.tables[directory_idx] |= 3;
        // PAGE_SET_ADDR( page_directory.tables[directory_idx], tphys );
        // PAGE_SET_BIT(page_directory.tables[directory_idx], PAGE_PRESENT, 1);
        // PAGE_SET_BIT(page_directory.tables[directory_idx], PAGE_RW, 1);
        page_directory->virtuals[directory_idx] = table;
    }
    page_dir_ent_t* te = &table->pages[table_idx];

    *(uint32_t*)te = phys | (*(uint32_t*)&flags) | 3;
    // PAGE_SET_ADDR(*te, 0);
    te->frame = phys >> 12 & 0x03FF;
    te->present = 1;
    te->user = flags.user;
    // te->rw = 1;
    // PAGE_SET_ADDR(*te, phys);
    // PAGE_SET_BIT(*te, PAGE_PRESENT, 1);
    // PAGE_SET_BIT(*te, PAGE_RW, 1);
    __invlpg_flush();
}

const void* next_virt()
{
    //https://compas.cs.stonybrook.edu/~nhonarmand/courses/fa17/cse306/slides/07-x86_vm.pdf
    return NULL;
}