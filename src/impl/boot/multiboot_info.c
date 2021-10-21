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
    memcpy( &__multiboot_info, __multiboot_info_temporary, sizeof(multiboot_info_t) );
    memset( __multiboot_info_temporary  , 0, sizeof(multiboot_info_t) );
    __multiboot_info_temporary = NULL;
    memcpy( &__multiboot_modules[0], (void*) __multiboot_info.mods_addr+0xc0000000, sizeof(module_t) * __multiboot_info.mods_count );
    __multiboot_info.mods_addr = (unsigned long) &__multiboot_modules;
    return OS32_SUCCESS;
}