#ifndef OS32_GDT
#define OS32_GDT
#include <stdint.h>
#include "stdlib/error.h"

/**
 * Perform gdt installation
 */
err_t __install_gdt();

/**
 * Set a gate of the gdt
 */
err_t gdt_set_gate( 
    int32_t number, 
    unsigned long base, 
    unsigned long limit, 
    uint8_t access, 
    uint8_t granularity 
);

/**
 * Flush the gdt (update everything)
 */
extern void gdt_flush();


#endif