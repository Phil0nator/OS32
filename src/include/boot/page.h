#ifndef OS32_PAGE_H
#define OS32_PAGE_H
#include <stdint.h>
#include <stddef.h>

#define PAGE_SIZE (4096)
#define HUGE_PAGE_SIZE (0x100000)



typedef uint32_t phys_addr;

#pragma pack(1)
typedef struct page_table_ent
{
    uint32_t present: 1;
    uint32_t rw: 1;
    uint32_t user: 1;
    uint32_t writethrough: 1;
    uint32_t nocache: 1;
    uint32_t accessed: 1;
    uint32_t size: 1;
    uint32_t ignored: 4;
    uint32_t addr: 20;
} page_table_ent_t;

typedef struct page_dir_ent
{
    uint32_t present: 1;
    uint32_t rw: 1;
    uint32_t user: 1;
    uint32_t accessed: 1;
    uint32_t dirty: 1;
    uint32_t reserved_bits: 7;
    uint32_t frame: 20;
} page_dir_ent_t;

typedef struct page_table
{
    page_table_ent_t pages[1024];
} page_table_t;

typedef struct page_dir
{
    page_dir_ent_t tables[1024];
    void* virtuals[1024];
} page_dir_t;

#pragma pack(0)

extern page_dir_t boot_page_directory;

phys_addr phys_addr_of( const void* virtual_addr );
void wire_page( phys_addr phys, const void* virt, page_table_ent_t flags );
const void* next_virt();

void set_cr3( phys_addr a );
phys_addr get_cr3();
extern void __invlpg_flush();

#endif