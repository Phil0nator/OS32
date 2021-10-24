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
    // if the heap is empty
    if (!kmalloc_heap_last)
    {
        // load this pointer as the sole member
        // of the heap
        kmalloc_heap_last = h;
        kmalloc_heap_first = h;
        return;
    }
    // Otherwise, push it to the end
    kmalloc_heap_last->next = h;
    h->prev = kmalloc_heap_last;
    h->next = NULL;
    kmalloc_heap_last = h;
}

void kmalloc_remove( struct kmalloc_header* h )
{
    // set to true if h is the first or last element of the heap
    bool first_or_last = false;

    if (h == kmalloc_heap_first)
    {
        // if h is the first element, the
        // kmalloc_heap_first pointer needs to be
        // updated 
        kmalloc_heap_first = h->next;
        if (kmalloc_heap_first)
        {
            kmalloc_heap_first->prev = NULL;
        }
        first_or_last = true;
    }
    if (h == kmalloc_heap_last)
    {
        // if h is the last element, the
        // kmalloc_heap_last pointer needs to be
        // updated 
        kmalloc_heap_last = h->prev;
        if (kmalloc_heap_last)
        {
            kmalloc_heap_last->next = NULL;
        }
        first_or_last = true;
    }
    // if h is somewhere in the middle,
    if (!first_or_last)
    {
        // splice it
        h->prev->next = h->next;
        h->next->prev = h->prev;
    }

    // just for ease of debugging
    h->next = NULL;
    h->prev = NULL;
}

/**
 * Split a block h into at least 1, normally two blocks.
 * 
 * If the second block to be produced would end up with a size less 
 * than or equal to zero, no split occurs.
 * @param h the block to split
 * @param size the desired size that h should be after this function
 */
void kmalloc_split( struct kmalloc_header* h, size_t size )
{
    int remaining_size = h->size - size - sizeof(struct kmalloc_header);
    // if the second block would not have a size greater than zero
    // return here and don't perform a split
    if (remaining_size <= (int)sizeof(struct kmalloc_header))
    {
        return;
    }
    // create a second block
    struct kmalloc_header* h2 = (struct kmalloc_header*)((((char*)h)+size+sizeof(struct kmalloc_header)));
    // setup the sizes
    h2->size = remaining_size ;
    h->size = size;
    // add the new block
    kmalloc_push(h2);
}
/**
 * Merge a block back into the heap.
 * 
 * If an existing block can simply be expanded to consume h, 
 * this is done here in order to mitigate fragmentation.
 * If no such block can be found, h is appended onto the end
 * of the heap.
 * @param h a block to add to the heap 
 */
void kmalloc_merge( struct kmalloc_header* h )
{
    struct kmalloc_header* it = kmalloc_heap_first;
    while (it)
    {
        // it := each element in the heap
        // if it is adjacent to h on its lesser side,
        if ((((uint32_t)it) + sizeof(struct kmalloc_header) + it->size) == (uint32_t)h)
        {
            // expand it to consume h
            it->size += sizeof(struct kmalloc_header) + h->size;
            return;
        }
        // else if it is adjacent to h on its greater side,
        else if ( (((uint32_t)h) + sizeof(struct kmalloc_header) + h->size) == (uint32_t)it )
        {
            // expand h to consume it
            h->size += sizeof(struct kmalloc_header) + it->size;
            kmalloc_push(h);
            kmalloc_remove(it);
            return;
        }
        // if at the end, break
        if (it->next == kmalloc_heap_last) break;
        it = it->next;
    }
    // If no merge opporitunities could be found,
    // append h to the end of the heap
    kmalloc_push(h);
}

// TODO: Alignment
struct kmalloc_header* kmalloc_alloc( size_t size, size_t align )
{
    struct kmalloc_header* ptr = kmalloc_heap_first;
    // kmalloc_stage is used to bootstrap the paging setup

    // first call
    if (kmalloc_stage == 0) {
        kmalloc_stage ++;
        return (struct kmalloc_header*) bootstrap_page;
    }
    // second call
    else if (kmalloc_stage == 1)
    {
        kmalloc_heap_first = kmalloc_heap_start;
        kmalloc_heap_first->size = kmalloc_heap_end - kmalloc_heap_start - sizeof(struct kmalloc_header); 
        kmalloc_heap_last = kmalloc_heap_first;
        ptr = kmalloc_heap_first;
        kmalloc_stage++;
    }
    // all further calls:
    
    // for each pointer in the heap... 
    while (ptr)
    {
        // if the pointer has enough space for the requested amount
        if (ptr->size >= size)
        {
            // slice off exactly the requested amount of memory
            kmalloc_split( ptr, size );
            // remove the pointer from the 'free' heap
            kmalloc_remove( ptr );
            // return the pointer
            return ptr;
        }
        // if at the end, break;
        if (ptr->next == kmalloc_heap_last) break;

        ptr=ptr->next;
    }
    // TODO: 
    // add a way to page more memory to expand the kernel heap
    return NULL;
}

// determine if a physical address is reserved or not
bool phys_available( phys_addr addr )
{
    return physical_present[addr/PAGE_SIZE] & (1<<(addr/PAGE_SIZE/8));
}
// reserve a physical address
void phys_reserve( phys_addr addr )
{
    physical_present[addr/PAGE_SIZE] |= ((1<<((addr/PAGE_SIZE)/8))-1);
}
// free a physical address
void phys_free(phys_addr addr)
{
    physical_present[addr/PAGE_SIZE] &= ~((1<<((addr/PAGE_SIZE)/8))-1);
}
// get the next available physical address
phys_addr kmalloc_next_phys( )
{
    // Traverse the bitfield, searching for the next 
    // unreserved physical address
    for (size_t i = 0; i < sizeof(physical_present); i++)
    {
        // if any pages of this group of 8 are reserved...
        if (physical_present[i])
        {   
            // check each page
            for (size_t j = 0; j < sizeof(char); j++)
            {
                // if any bit is not set,
                if (!BIT_N_OF_X((physical_present[i]), j))
                {
                    // return the corresponding page
                    return (i+j)*PAGE_SIZE;
                }
            }
        }
        // this whole set of 8 pages is free,
        else
        {
            // return the first
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
    // flush cpu paging system
    set_cr3(get_cr3());
}

// reserve a set of pages
void phys_reserve_pages( phys_addr start, phys_addr end )
{
    for (;start < end; start += PAGE_SIZE)
    {
        phys_reserve(start);
    }
}

err_t __install_kmalloc()
{
    // setup boundaries
    phys_begin = KERNEL_PHYS_END;
    phys_alloc_ptr = phys_begin;
    phys_end = phys_begin+__multiboot_info.mem_upper*1000;

    // reserve the first chunk of physical memory for:
    // - reserved low space (BIOS, etc...)
    // - kernel space
    memset( physical_present, 0xff,(( 0x100000 + (KERNEL_PHYS_END-KERNEL_PHYS_START))/PAGE_SIZE)/8 );

    // reserve memory for modules
    for ( size_t i = 0; i < __multiboot_info.mods_count; i++ )
    {
        module_t* mod = &((module_t*)__multiboot_info.mods_addr)[i];
        phys_reserve_pages( mod->mod_start, mod->mod_end );
        
    }

    // allocate 100 pages for the heap to start off with
    kmalloc_alloc_pages( 100ul, (void*) KERNEL_HEAP_START, (page_table_ent_t){0} );
    // zero out the entire heap to start off
    memset(kmalloc_heap_start, 0, 100*PAGE_SIZE);

    return OS32_SUCCESS;
}


kmalloc_ptr kmalloc_page_struct( phys_addr* phys_dest )
{
    char* virt = (char*)kmalloc_alloc( PAGE_SIZE, PAGE_SIZE );
    // for bootstrapping:
    // on the first call, the statically allocated page is returned,
    // which is less than the KERNEL_HEAP_START number.
    // So, for the first call, the virtual address' offset from the kernel's
    // virtual address is sufficient.
    // Otherwise, the virtual address of the can just be given to the
    // phys_addr_of function 
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