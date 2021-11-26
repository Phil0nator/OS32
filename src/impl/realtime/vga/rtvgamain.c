#include "realtime/vga/rtvgamain.h"
#include "realtime/vga/utils.h"
#include "drivers/rtc.h"
#include "realtime/vga/topbar.h"
#include "drivers/timer.h"
#include "stdlib/string.h"

#include "system/filesystems/vfs.h"
#include "system/process/multitasking.h"


static const char testerchar[81] = {'\0'};
process_t* proc;


void rtvgamain()
{

    pid_t pid = __spawn("/initrd/bin/test");
    proc = get_proc_by_id(pid);
    while (proc->eip > 0xc0000000)
    {
        pit_waitt( 5 );
    }
    // memset(testerchar, 205, 80 );
    rtvga_cls( VGA_BLACK );
    while(1)
    {
        rtvga_topbar_flip();
        char buffer[1024] = {0};
        vfs_read( proc->local_fdt[1], buffer, 1024 );
        vgaPuts(buffer);
    }

    for(;;);
}