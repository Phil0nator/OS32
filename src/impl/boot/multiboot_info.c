#include "boot/mutiboot_info.h"
#include "stdlib/string.h"
multiboot_info_t* __multiboot_info_temporary;
multiboot_info_t __multiboot_info;
module_t __multiboot_modules[16];

err_t __load_multiboot()
{
    if (__multiboot_info_temporary == NULL)
    {
        return OS32_ERROR;
    }
    // copy the original multiboot structure into kernelspace
    memcpy( &__multiboot_info, __multiboot_info_temporary, sizeof(multiboot_info_t) );
    // reset original data to null
    memset( __multiboot_info_temporary, 0, sizeof(multiboot_info_t) );
    // remove pointer to old original
    __multiboot_info_temporary = NULL;
    // copy all the modules into kernel space
    // (the number 0xc0000000) is for the paging offset
    memcpy( &__multiboot_modules[0], (void*) __multiboot_info.mods_addr+0xc0000000, sizeof(module_t) * __multiboot_info.mods_count );
    // update the mods_addr pointer in the struct to the modules' new location 
    __multiboot_info.mods_addr = (unsigned long) &__multiboot_modules;
    return OS32_SUCCESS;
}