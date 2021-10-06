#include "stdlib/kmalloc.h"
#include "boot/mutiboot_info.h"
#include "boot/vga.h"
#include "boot/page.h"
#include <stdbool.h>

#define MEM_UPPER_START (1000000)


extern volatile phys_addr _kernel_start;
extern volatile phys_addr _kernel_end;

static phys_addr phys_begin = MEM_UPPER_START; // 1MB
static phys_addr phys_alloc_ptr;
static phys_addr phys_end;

err_t __install_kmalloc()
{
    phys_begin += _kernel_end-_kernel_start;
    phys_alloc_ptr = phys_begin;
    phys_end = phys_begin+__multiboot_info.mem_upper*1000;
    
    // vgaPrintf("pstart: %u, pend: %u, psize: %u\n", phys_begin, phys_end, phys_end-phys_begin);
    
    return OS32_SUCCESS;
}

static phys_addr kmalloc_next_phys( bool huge )
{
    phys_addr out = phys_alloc_ptr;
    phys_alloc_ptr += huge ? HUGE_PAGE_SIZE : PAGE_SIZE;
    if (out >= phys_end)
    {
        return OS32_ERROR;
    }
    return out;
}




kmalloc_ptr kmalloc( size_t size )
{
    phys_addr phys = kmalloc_next_phys(false);
    vgaPrintf("PHYS: %u\n", phys);
    const void* ptr = next_virt();
    vgaPrintf("VIRT: %u\n", ptr);

    wire_page( phys, ptr, (page_table_ent_t){0} );

    return (kmalloc_ptr)(ptr);

}
void kfree( kmalloc_ptr ptr )
{

}
void kprotect( kmalloc_ptr ptr)
{

}