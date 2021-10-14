#ifndef OS32_KMALLOC
#define OS32_KMALLOC
#include "error.h"
#include "boot/page.h"
#include <stdint.h>
#include <stddef.h>

typedef void* kmalloc_ptr;


err_t __install_kmalloc();

/**
 * Allocate a certain number of physical pages to a given virtual address with given permissions
 * @param count number of physical pages
 * @param virtual_addr desired virtual address of first page
 * @param perms permissions applied to the pages
 */
void kmalloc_alloc_pages( size_t count, void* virtual_addr, page_table_ent_t perms );
/**
 * Free allocated physical pages
 * @param count number of physical pages
 * @param virtual_addr virtual address of first physical page
 */
void free_pages( size_t count, void* virtual_addr );

/**
 * kmalloc a page aligned page, and return the physical address that corresponds to the returned virtual address.
 * Blocks returned by kmalloc_page_struct() can be freed using kfree like any other kmalloc_ptr.
 * @param phys_dest scribble location
 * @returns a virtual address to a newly allocated page aligned page
 */
kmalloc_ptr kmalloc_page_struct( uint32_t* phys_dest );

/**
 * Allocate a piece of memory of a certain size
 * @param size number of bytes to allocate
 * @returns a valid virtual address pointing to at least 'size' available bytes
 */
kmalloc_ptr kmalloc( size_t size );
/**
 * free memory returned by kmalloc or friends
 * @param ptr value returned by kmalloc or friends
 * @warning passing NULL as the parameter will cause no error
 */
void kfree( kmalloc_ptr ptr );
/**
 * Reallocate a given kmalloc pointer to a given size.
 * This can both grow and shrink allocations.
 * The return value is not guarenteed to be the same as 'ptr'.
 * @param ptr memory to reallocate
 * @param size desired size
 * @returns a valid pointer to at least 'size' bytes of available memory
 */
kmalloc_ptr krealloc( kmalloc_ptr ptr, size_t size );
/**
 * Allocate a piece of memory of a certain size and zero-initializes it
 * @param size number of bytes to allocate
 * @returns a valid virtual address pointing to at least 'size' available bytes initialized to zero
 */
kmalloc_ptr kcalloc( size_t size );
/**
 * Set the permissions for a given kmalloc pointer
 */
void kprotect( kmalloc_ptr ptr );

/// Diagnostics

/**
 * Get the total amount of memory owned by the kmalloc context
 */
size_t kmalloc_volume();

/**
 * Completely defragment the kmalloc context as much as possible
 */
void kmalloc_defrag();

#endif