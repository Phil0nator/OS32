#include "system/filesystems/linitrd.h"
#include "boot/mutiboot_info.h"

struct ext2_partition* __initrd;

err_t __install_initrd()
{
    module_t* mods = (module_t*) __multiboot_info.mods_addr;
    char* raw = (char*) mods[0].mod_start + 0xc0000000;
    ext2_init( &__initrd, raw );

    return OS32_SUCCESS;
}