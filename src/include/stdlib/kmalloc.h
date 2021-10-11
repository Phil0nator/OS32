#ifndef OS32_KMALLOC
#define OS32_KMALLOC
#include "error.h"
#include <stdint.h>
#include <stddef.h>

typedef void* kmalloc_ptr;


err_t __install_kmalloc();


void free_pages( size_t count, void* virtual_addr );


kmalloc_ptr kmalloc_page_struct( uint32_t* phys_dest );
kmalloc_ptr kmalloc( size_t size );
void kfree( kmalloc_ptr ptr );
void kprotect( kmalloc_ptr ptr );

#endif