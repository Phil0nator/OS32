#include "system/filesystems/linitrd.h"
#include "boot/mutiboot_info.h"

struct ext2_partition* __initrd;

err_t __install_initrd()
{
    // find modules
    module_t* mods = (module_t*) __multiboot_info.mods_addr;
    // apply paging offset
    char* raw = (char*) mods[0].mod_start + 0xc0000000;
    // create the ext2 partition
    if (ext2_init( &__initrd, raw ) != OS32_SUCCESS)
    {
        return OS32_ERROR;
    }

    return OS32_SUCCESS;
}