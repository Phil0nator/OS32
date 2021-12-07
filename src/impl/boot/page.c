#include "boot/page.h"
#include "stdlib/assert.h"
#include "stdlib/kmalloc.h"
#include "stdlib/instructions.h"
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

// Set the value held by the cr3 register
extern void set_cr3( phys_addr a );
// Get the value held by the cr3 register
extern phys_addr get_cr3();

extern void cpy_phys_pgs( phys_addr d, phys_addr s );

extern page_dir_t boot_page_directory;
page_dir_t* current_page_directory = &boot_page_directory;

phys_addr phys_addr_of(page_dir_t* page_directory, const void* virtual_addr )
{
    virtual_addr = (void*)PAGE_ALIGNED(virtual_addr);
    // va_conv_t converter;
    // memcpy( &converter, &virtual_addr, sizeof(virtual_addr) );
    // // *(uint32_t*)(&converter) = (uint32_t) virt;
    // uint32_t directory_idx = converter.di;
    // uint32_t table_idx = converter.ti;
    // uint32_t directory_idx = ((uint32_t)virtual_addr) >> 22;
    // uint32_t table_idx = ((uint32_t)virtual_addr) >> 12 & 0x03FF;
    uint32_t directory_idx, table_idx;
    virt_interp(virtual_addr, &directory_idx, &table_idx);
    

    page_table_t* table = (page_table_t*) page_directory->virtuals[directory_idx];
    if (!table)
    {
        return NULL;
    }
    return table->pages[table_idx].frame * PAGE_SIZE;
}


void unwire_page( page_dir_t* page_directory, const void* virt)
{
    // va_conv_t converter;
    // memcpy( &converter, &virt, sizeof(virt) );
    // *(uint32_t*)(&converter) = (uint32_t) virt;
    // uint32_t directory_idx = converter.di;
    // uint32_t table_idx = converter.ti;
    uint32_t directory_idx, table_idx;
    virt_interp(virt, &directory_idx, &table_idx);
    
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
    flushcr3();
}

static void clone_table( page_dir_t* dest, size_t index, const page_dir_t* source )
{
    phys_addr phys;
    page_table_t* newpg = kmalloc_page_struct(&phys);
    memcpy(newpg, source->virtuals[index], sizeof(page_table_t));
    dest->tables[index] = source->tables[index];
    dest->tables[index].frame = PAGE_ALIGNED(phys)/PAGE_SIZE;
    dest->virtuals[index] = newpg;

    for (size_t i = 0; i < 1024; i ++)
    {
        page_table_t* srct = ((page_table_t*)source->virtuals[index]);
        if (srct->pages[i].present)
        {
            // va_conv_t cv;
            // cv.di = index;
            // cv.ti = i;
            // cv.off = 0;
            // void* virt = *(void**)(&cv);
            void* virt = 0 | (index << 22) | ( i << 12 );
            page_dir_ent_t perms = srct->pages[i];
            kmalloc_alloc_pages( dest, 1, virt, (page_table_ent_t){ 
                .present = perms.present,  
                .rw = perms.rw,
                .user = perms.user,
                .accessed = perms.accessed
                } );
            
            phys_addr phys_dest, phys_src;
            phys_dest = newpg->pages[i].frame * PAGE_SIZE;
            phys_src = srct->pages[i].frame * PAGE_SIZE;
            
            eflags_t efl;
            __get_eflags(efl);
            __cli
            wire_page(current_page_directory, phys_dest, phys_dest, (page_table_ent_t){.present=1,.rw=1});
            wire_page(current_page_directory, phys_src, phys_src, (page_table_ent_t){.present=1,.rw=1});
            memcpy(phys_dest, phys_src, PAGE_SIZE);
            unwire_page(current_page_directory, phys_dest);
            unwire_page(current_page_directory, phys_src);
            __set_eflags(efl);
        }
    }
}

void dir_dup( page_dir_t* dest, const page_dir_t* src )
{
    for (size_t i = 0; i < 1024; i++)
    {
        if (src->tables[i].present)
        {
            if (boot_page_directory.tables[i].present)
            {
                dest->tables[i] = src->tables[i];
                dest->virtuals[i] = src->virtuals[i];
            }
            else
            {
                clone_table( dest, i, src );
            }
        }
    }
}

void virt_interp( void* virt, uint32_t* di, uint32_t* ti )
{
    *di = (uint32_t)virt >> 22;
    *ti = (uint32_t)virt >> 12 & 1023;
}

void wire_page( page_dir_t* page_directory, phys_addr phys, const void* virt, page_table_ent_t flags )
{
    // virt = (char*)((uint32_t)virt & (~0x03ff));
    // virt = (const void*) PAGE_ALIGNED(virt);
    // va_conv_t converter;
    // memcpy( &converter, &virt, sizeof(virt) );
    // // *(uint32_t*)(&converter) = (uint32_t) virt;
    // uint32_t directory_idx = converter.di;
    // uint32_t table_idx = converter.ti;
    vga_assert(PAGE_ALIGNED(virt) == virt);

    uint32_t directory_idx, table_idx;
    virt_interp(virt, &directory_idx, &table_idx);

    vga_assert(table_idx <= 1024);
    vga_assert(directory_idx <= 1024);

    page_table_t* table = (page_table_t*) page_directory->virtuals[directory_idx];
    if (!table)
    {
        phys_addr tphys;
        table = kmalloc_page_struct((phys_addr*)(&tphys));
        vga_assert(PAGE_ALIGNED(table) == table);
        vga_assert(PAGE_ALIGNED(tphys) == tphys);
        memset(table, 0, PAGE_SIZE);
        page_directory->tables[directory_idx].frame = PAGE_ALIGNED(tphys)/PAGE_SIZE;
        
        // *(uint32_t*)&page_directory->tables[directory_idx] = PAGE_ALIGNED(tphys)/PAGE_SIZE;
        page_directory->tables[directory_idx].present = 1;
        page_directory->tables[directory_idx].rw = 1;
        page_directory->tables[directory_idx].user = flags.user;

        page_directory->virtuals[directory_idx] = table;
    }
    page_dir_ent_t* te = &table->pages[table_idx];
    *(uint32_t*)te = 0;
    // *(uint32_t*)te = (phys&0x03ff) | (*(uint32_t*)&flags) | 3;
    // PAGE_SET_ADDR(*te, 0);
    te->frame = PAGE_ALIGNED(phys)/PAGE_SIZE;
    te->present = 1;
    te->user = flags.user;
    te->rw = flags.rw;
    // te->rw = 1;
    // PAGE_SET_ADDR(*te, phys);
    // PAGE_SET_BIT(*te, PAGE_PRESENT, 1);
    // PAGE_SET_BIT(*te, PAGE_RW, 1);
    __invlpg_flush();
    set_cr3(get_cr3());
}



page_dir_t* set_pd( page_dir_t* pd )
{
    page_dir_t* old = current_page_directory;
    current_page_directory = pd;
    set_cr3(current_page_directory->phys);
    return old;
}
page_dir_t* get_pd(  )
{
    return current_page_directory;
}

page_dir_t* mkpd( page_dir_t* parent )
{
    page_dir_t* out = kmalloc_a( sizeof(page_dir_t), PAGE_SIZE );
    memset(out, 0, sizeof(page_dir_t));
    out->phys = phys_addr_of( current_page_directory, out );
    dir_dup( out, parent ? parent : current_page_directory );
    return out;
}

void flushcr3()
{
    set_cr3(get_cr3());
}