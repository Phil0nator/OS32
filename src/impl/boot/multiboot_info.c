#include "boot/mutiboot_info.h"
#include "stdlib/string.h"
multiboot_info_t* __multiboot_info_temporary;
multiboot_info_t __multiboot_info;

err_t __load_multiboot()
{
    if (__multiboot_info_temporary == NULL)
    {
        return OS32_ERROR;
    }
    memcpy( &__multiboot_info, __multiboot_info_temporary, sizeof(multiboot_info_t) );
    memset( __multiboot_info_temporary  , 0, sizeof(multiboot_info_t) );
    __multiboot_info_temporary = NULL;
    return OS32_SUCCESS;
}