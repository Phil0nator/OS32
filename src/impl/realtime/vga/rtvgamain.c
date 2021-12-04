#include "realtime/vga/rtvgamain.h"
#include "realtime/vga/utils.h"
#include "drivers/rtc.h"
#include "realtime/vga/topbar.h"
#include "drivers/timer.h"
#include "stdlib/string.h"

#include "system/filesystems/vfs.h"
#include "system/process/multitasking.h"
#include "realtime/vga/termproc.h"
#include "drivers/keyboard.h"

static const char testerchar[81] = {'\0'};
process_t* proc;

void onkeypress( uint32_t scancode, char ASCII, os32_kb_action_t action )
{
    if (ASCII && action == KB_RELEASE)
        vfs_write( proc->local_fdt[0], &ASCII, 1 );
}

void rtvgamain()
{
    vgaSetCursor( 0, 1 );
    vgaShowCursor(true);
    os32_keyboard_set_routine( onkeypress );

    pid_t pid = __spawn("/initrd/bin/sh");
    pit_waitt(1);
    proc = get_proc_by_id(pid);
    rtvga_termproc_t* first_proc = kmalloc(sizeof(rtvga_termproc_t));
    termproc_create( first_proc );
    first_proc->pid = pid;
    // for(;;);
    // memset(testerchar, 205, 80 );
    rtvga_cls( VGA_WHITE, VGA_BLACK );
    while(1)
    {
        rtvga_topbar_flip();
        proc = get_proc_by_id((*rtvga_curproc)->pid);
        size_t new_data = 0;
        for (size_t i = 0; i < 3; i++)
        {
            char buffer[256] = {0};
            size_t amt = vfs_read( proc->local_fdt[i], buffer, 256 );
            new_data+=amt;
            switch (i)
            {
            case 0:
                streambuf_write(&(*rtvga_curproc)->m_stdin, buffer, amt);
                break;
            case 1:
                streambuf_write(&(*rtvga_curproc)->m_stdout, buffer, amt);
                break;
            case 2:
                streambuf_write(&(*rtvga_curproc)->m_stderr, buffer, amt);
                break;
            default:
                break;
            }
        }
        if (new_data)
        {
            termproc_display(*rtvga_curproc);
        }
    }

    for(;;);
}