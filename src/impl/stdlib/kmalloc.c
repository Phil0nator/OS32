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
static void* kmalloc_heap_start = (void*) KERNEL_HEAP_START;
// (the heap starts as 100 pages)
static void* kmalloc_heap_end = (void*) KERNEL_HEAP_START+(PAGE_SIZE*100);

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

static struct kmalloc_header* kmalloc_heap_first;
static struct kmalloc_header* kmalloc_heap_last;


void kmalloc_push( struct kmalloc_header* h )
{
    if (!kmalloc_heap_last)
    {
        kmalloc_heap_last = h;
        kmalloc_heap_first = h;
        return;
    }
    kmalloc_heap_last->next = h;
    h->prev = kmalloc_heap_last;
    h->next = NULL;
    kmalloc_heap_last = h;
}

void kmalloc_remove( struct kmalloc_header* h )
{
    bool first_or_last = false;
    if (h == kmalloc_heap_first)
    {
        kmalloc_heap_first = h->next;
        if (kmalloc_heap_first)
        {
            kmalloc_heap_first->prev = NULL;
        }
        first_or_last = true;
    }
    if (h == kmalloc_heap_last)
    {
        kmalloc_heap_last = h->prev;
        if (kmalloc_heap_last)
        {
            kmalloc_heap_last->next = NULL;
        }
        first_or_last = true;
    }
    if (!first_or_last)
    {
        h->prev->next = h->next;
        h->next->prev = h->prev;
    }

    
    h->next = NULL;
    h->prev = NULL;
}

void kmalloc_split( struct kmalloc_header* h, size_t size )
{
    int remaining_size = h->size - size - sizeof(struct kmalloc_header);
    if (remaining_size <= (int)sizeof(struct kmalloc_header))
    {
        return;
    }
    struct kmalloc_header* h2 = (struct kmalloc_header*)((((char*)h)+size+sizeof(struct kmalloc_header)));
    h2->size = remaining_size ;
    h->size = size;
    kmalloc_push(h2);
}

void kmalloc_merge( struct kmalloc_header* h )
{
    struct kmalloc_header* it = kmalloc_heap_first;
    while (it)
    {
        if ((((uint32_t)it) + sizeof(struct kmalloc_header) + it->size) == (uint32_t)h)
        {
            it->size += sizeof(struct kmalloc_header) + h->size;
            return;
        }
        else if ( (((uint32_t)h) + sizeof(struct kmalloc_header) + h->size) == (uint32_t)it )
        {
            h->size += sizeof(struct kmalloc_header) + it->size;
            kmalloc_push(h);
            kmalloc_remove(it);
            return;
        }
        if (it->next == kmalloc_heap_last) break;
        it = it->next;
    }
    kmalloc_push(h);
}

struct kmalloc_header* kmalloc_alloc( size_t size, size_t align )
{
    struct kmalloc_header* ptr = kmalloc_heap_first;
    if (kmalloc_stage == 0) {
        kmalloc_stage ++;
        return (struct kmalloc_header*) bootstrap_page;
    }
    else if (kmalloc_stage == 1)
    {
        kmalloc_heap_first = kmalloc_heap_start;
        kmalloc_heap_first->size = kmalloc_heap_end - kmalloc_heap_start - sizeof(struct kmalloc_header); 
        kmalloc_heap_last = kmalloc_heap_first;
        ptr = kmalloc_heap_first;
        kmalloc_stage++;
    }
    while (ptr)
    {
        if (ptr->size >= size)
        {
            kmalloc_split( ptr, size );
            kmalloc_remove( ptr );
            return ptr;
        }
        if (ptr->next == kmalloc_heap_last) break;

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
    return 0;
}

void kmalloc_alloc_pages( size_t count, void* virtual_addr, page_table_ent_t perms )
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

err_t __install_kmalloc()
{
    phys_begin = KERNEL_PHYS_END;
    phys_alloc_ptr = phys_begin;
    phys_end = phys_begin+__multiboot_info.mem_upper*1000;

    memset( physical_present, 0xff,(( 0x100000 + (KERNEL_PHYS_END-KERNEL_PHYS_START))/PAGE_SIZE)/8 );


    kmalloc_alloc_pages( 100ul, (void*) KERNEL_HEAP_START, (page_table_ent_t){0} );
    memset(kmalloc_heap_start, 0, 100*PAGE_SIZE);


    return OS32_SUCCESS;
}


kmalloc_ptr kmalloc_page_struct( phys_addr* phys_dest )
{
    char* virt = (char*)kmalloc_alloc( PAGE_SIZE, PAGE_SIZE );
    if (virt > (char*)KERNEL_HEAP_START)
        *phys_dest = phys_addr_of(virt);
    else
        *phys_dest = virt-(char*)KERNEL_VIRTUAL; 
    return virt;
}


kmalloc_ptr kmalloc( size_t size )
{

    struct kmalloc_header* h = kmalloc_alloc( size, 1 );
    return ((char*)h)+sizeof(struct kmalloc_header);
}
void kfree( kmalloc_ptr ptr )
{
    if (!ptr) return;
    kmalloc_merge((struct kmalloc_header*)(((char*)ptr)-sizeof(struct kmalloc_header)));
}
void kprotect( kmalloc_ptr ptr)
{
    struct kmalloc_header* h = (struct kmalloc_header*)(((char*)ptr)-sizeof(struct kmalloc_header));
}
kmalloc_ptr krealloc( kmalloc_ptr ptr, size_t size )
{
    // DNE
    if (!ptr) return kmalloc(size);
    struct kmalloc_header* header = (struct kmalloc_header*)((char*)ptr)-sizeof(struct kmalloc_header);
    if ( header->size == size )
    {
        // same
        return ptr;
    }
    else if (header->size < size)
    {
        // trunc
        kmalloc_split(header, size);
        return ptr;
    }
    else
    {
        // expand
        kmalloc_ptr nptr = kmalloc(size);
        memcpy(nptr, ptr, header->size);
        kfree( ptr );
        return nptr;
    }
}
kmalloc_ptr kcalloc( size_t size )
{
    kmalloc_ptr out = kmalloc( size );
    memset( out, 0, size );
    return out;
}
size_t kmalloc_volume()
{
    struct kmalloc_header* it = kmalloc_heap_first;
    size_t counter = 0;
    while (it)
    {
        counter += sizeof(struct kmalloc_header) + it->size;
        if (kmalloc_heap_last == it->next) break;
        it = it->next;
    }
    return counter;
}

struct kmalloc_header* kmalloc_header_index( size_t idx )
{
    struct kmalloc_header* it = kmalloc_heap_first;
    while (it && idx)
    {
        idx--;
        if (kmalloc_heap_last == it->next) break;
        it = it->next;
    }
    return it;
}

void kmalloc_defrag()
{
    size_t counter = 0;
    struct kmalloc_header* it = NULL;
    while ( (it = kmalloc_header_index(counter++)) != NULL )
    {
        kmalloc_remove(it);
        kmalloc_merge(it);
    }
}