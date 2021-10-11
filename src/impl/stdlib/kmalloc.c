#include "stdlib/kmalloc.h"
#include "boot/mutiboot_info.h"
#include "boot/vga.h"
#include "boot/page.h"
#include "stdlib/string.h"
#include "stdlib/ioinstrs.h"
#include <stdbool.h>

/**
 * Memory constants
 */

// upper memory
#define MEM_UPPER_START (1000000)
// higher half start
#define KERNEL_VIRTUAL (0xc0000000)
// Chosen kernel heap virtual address-space
#define KERNEL_HEAP_START (0xd0000000)
// Chosen kernel heap physical address-space start
#define KERNEL_PHYS_START (0x100000)
// Chosen kernel heap physical address-space end
#define KERNEL_PHYS_END (0x200000)

// Get the nth bit of x
#define BIT_N_OF_X(x, n) (bool)((x) & (1<<(n)))


// linker-defined
extern volatile phys_addr* _kernel_start;
extern volatile phys_addr* _kernel_end;

// physical memory definitions
static phys_addr phys_begin; // 1MB
static phys_addr phys_alloc_ptr;
static phys_addr phys_end;

// physical memory tracker bitfield
static char physical_present[PAGE_SIZE * 32];

// virtual address-space heap management
static void* kmalloc_heap_start = KERNEL_HEAP_START;
// (the heap starts as 100 pages)
static void* kmalloc_heap_end = KERNEL_HEAP_START+(PAGE_SIZE*100);

// the initial page returned by kmalloc to set up paging
static char bootstrap_page[PAGE_SIZE] __attribute__ ((aligned (PAGE_SIZE)));
// bootstrap stage
static int kmalloc_stage = 0;


/**
 * Standard linked list malloc implimentation
 */
struct kmalloc_header
{
    struct kmalloc_header* next;
    struct kmalloc_header* prev;
    // size = (actual size) - sizeof(kmalloc_header)
    size_t size;
};

static struct kmalloc_header* kmalloc_heap;


void kmalloc_push( struct kmalloc_header* h )
{
    h->prev = kmalloc_heap->prev;
    h->next = NULL;
    if (h->prev)
        h->prev->next = h;
    kmalloc_heap->prev = h;
}

void kmalloc_remove( struct kmalloc_header* h )
{
    if (h->prev)
    {
        h->prev->next = h->next;
    }
    if (h->next)
    {
        h->next->prev = h->prev;
    }
    if (h == kmalloc_heap)
    {
        kmalloc_heap = h->prev;
    }
    h->prev = NULL;
    h->next = NULL;
}

void kmalloc_split( struct kmalloc_header* h, size_t size )
{
    int remaining_size = h->size - size - sizeof(struct kmalloc_header);
    if (remaining_size <= sizeof(struct kmalloc_header))
    {
        return;
    }
    struct kmalloc_header* h2 = (struct kmalloc_header*)(((char*)h)+size);
    h2->size = h->size - size - sizeof(struct kmalloc_header);
    h->size = size;
    kmalloc_push(h2);
}

void kmalloc_merge( struct kmalloc_header* h )
{
    struct kmalloc_header* it = kmalloc_heap;
    while (it)
    {
        if (((uint32_t)it + sizeof(struct kmalloc_header) + it->size) == h)
        {
            it->size += sizeof(struct kmalloc_header) + h->size;
            return;
        }
        it = it->next;
    }
    kmalloc_push(h);
}

struct kmalloc_header* kmalloc_alloc( size_t size, size_t align )
{
    struct kmalloc_header* ptr = kmalloc_heap;
    if (kmalloc_stage == 0) {
        kmalloc_stage ++;
        return bootstrap_page;
    }
    else if (kmalloc_stage == 1)
    {
        kmalloc_heap = kmalloc_heap_start;
        kmalloc_heap->size = kmalloc_heap_end - kmalloc_heap_start - sizeof(struct kmalloc_header); 
        kmalloc_heap->prev = kmalloc_heap;
        ptr = kmalloc_heap;
        kmalloc_stage++;
    }
    while (ptr)
    {
        if (ptr->size >= size && ( (size_t)ptr+sizeof(struct kmalloc_header) ) % align == 0 )
        {
            kmalloc_split( ptr, size );
            kmalloc_remove( ptr );
            return ptr;
        }
        ptr=ptr->next;
    }
    return NULL;
}


bool phys_available( phys_addr addr )
{
    return physical_present[addr/PAGE_SIZE] & (1<<(addr/PAGE_SIZE/8));
}

void phys_reserve( phys_addr addr )
{
    physical_present[addr/PAGE_SIZE] |= ((1<<((addr/PAGE_SIZE)/8))-1);
}

void phys_free(phys_addr addr)
{
    physical_present[addr/PAGE_SIZE] &= ~((1<<((addr/PAGE_SIZE)/8))-1);
}

phys_addr kmalloc_next_phys( )
{

    for (size_t i = 0; i < sizeof(physical_present); i++)
    {
        if (physical_present[i])
        {   
            for (size_t j = 0; j < sizeof(char); j++)
            {
                if (!BIT_N_OF_X((physical_present[i]), j))
                {
                    return (i+j)*PAGE_SIZE;
                }
            }
        }
        else
        {
            return i*PAGE_SIZE;
        }
    }

}


err_t __install_kmalloc()
{
    phys_begin = KERNEL_PHYS_END;
    phys_alloc_ptr = phys_begin;
    phys_end = phys_begin+__multiboot_info.mem_upper*1000;

    memset( physical_present, 0xff,(( 0x100000 + (KERNEL_PHYS_END-KERNEL_PHYS_START))/PAGE_SIZE)/8 );


    alloc_pages( 100, KERNEL_HEAP_START, (page_table_ent_t){NULL} );
    memset(kmalloc_heap_start, 0, 100*PAGE_SIZE);


    return OS32_SUCCESS;
}


kmalloc_ptr kmalloc_page_struct( phys_addr* phys_dest )
{
    char* virt = kmalloc_alloc( PAGE_SIZE, PAGE_SIZE );
    if (virt > KERNEL_HEAP_START)
        *phys_dest = phys_addr_of(virt);
    else
        *phys_dest = virt-KERNEL_VIRTUAL; 
    return virt;
}

// - one central physical allocator
// - multiple specialize virtual address 'choosers'
// - system to wire the pages
// - proto malloc for before proper setup

void alloc_pages( size_t count, void* virtual_addr, page_table_ent_t perms )
{
    // setup
    while (count--)
    {

        phys_addr phys = kmalloc_next_phys();
        phys_reserve( phys );

        void* virt = virtual_addr;
        virtual_addr += PAGE_SIZE;

        wire_page( phys, virt, perms );
    }
    set_cr3(get_cr3());
}
kmalloc_ptr kmalloc( size_t size )
{

    struct kmalloc_header* h = kmalloc_alloc( size, 1 );
    return h+1;
}
void kfree( kmalloc_ptr ptr )
{
    kmalloc_merge(ptr);
}
void kprotect( kmalloc_ptr ptr)
{

}