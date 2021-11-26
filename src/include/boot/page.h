#ifndef OS32_PAGE_H
#define OS32_PAGE_H
#include <stdint.h>
#include <stddef.h>

// size of 1 4K page
#define PAGE_SIZE (4096)
// size of 1 4M page
#define HUGE_PAGE_SIZE (0x100000)

#define PAGE_INTIZE( ent ) (*(uint32_t*)(&ent))
#define PAGE_SET_ADDR(ent, addr)(PAGE_INTIZE(ent) = (addr))
#define PAGE_SET_BIT( ent, bit, val ) (PAGE_INTIZE(ent) = (val) ? (PAGE_INTIZE(ent)|(1<<bit)) : (PAGE_INTIZE(ent)&~(1<<bit)) )

#define PAGE_ALIGNED(addr) ( ((uint32_t)addr)&(~(PAGE_SIZE-1)) )

#define PAGE_PRESENT    0
#define PAGE_RW         1
#define PAGE_USER       2

typedef uint32_t phys_addr;

#pragma pack(1)
typedef struct page_table_ent
{
    uint32_t present:       1;
    uint32_t rw:            1;
    uint32_t user:          1;
    uint32_t writethrough:  1;
    uint32_t nocache:       1;
    uint32_t accessed:      1;
    uint32_t size:          1;
    uint32_t ignored:       4;
    uint32_t addr:          20;
} page_table_ent_t;

typedef struct page_dir_ent
{
    uint32_t present:       1;
    uint32_t rw:            1;
    uint32_t user:          1;
    uint32_t accessed:      1;
    uint32_t dirty:         1;
    uint32_t reserved_bits: 7;
    uint32_t frame:         20;
} page_dir_ent_t;

typedef struct page_table
{
    page_dir_ent_t pages[1024];
} page_table_t;

typedef struct page_dir
{
    page_dir_ent_t tables[1024];
    void* virtuals[1024];
} page_dir_t;

#pragma pack(0)

extern page_dir_t boot_page_directory;

// Convert a valid virtual address to a physical address
phys_addr phys_addr_of(page_dir_t* page_directory, const void* virtual_addr );

// Wire a physical and virtual page with the given permissions
void wire_page(page_dir_t* page_directory, phys_addr phys, const void* virt, page_table_ent_t flags );

void unwire_page( page_dir_t* page_directory, const void* virt);

// DEPRICATED
const void* next_virt();

// Duplicate an address-space
void dir_dup( page_dir_t* dest, const page_dir_t* src );

// Set the value held by the cr3 register
void set_cr3( phys_addr a );
// Get the value held by the cr3 register
phys_addr get_cr3();
// flush paging system (update)
extern void __invlpg_flush();

#endif