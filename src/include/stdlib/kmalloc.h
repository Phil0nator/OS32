#ifndef OS32_KMALLOC
#define OS32_KMALLOC
#include "error.h"
#include <stdint.h>
#include <stddef.h>

typedef void* kmalloc_ptr;




kmalloc_ptr kmalloc( size_t size );
void kfree( kmalloc_ptr ptr );
void kprotect( kmalloc_ptr ptr );

#endif