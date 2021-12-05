#include "realtime/vga/rtvgamain.h"
#include "realtime/vga/utils.h"
#include "drivers/rtc.h"
#include "realtime/vga/topbar.h"
#include "drivers/timer.h"
#include "stdlib/string.h"
#include "stdlib/list.h"
#include "stdlib/instructions.h"

#include "system/filesystems/vfs.h"
#include "system/process/multitasking.h"
#include "realtime/vga/termproc.h"
#include "drivers/keyboard.h"
#include "system/sync/spinlock.h"

static const char testerchar[81] = {'\0'};
process_t* cur_proc;

typedef struct stdinbuf
{
    pid_t pid;
    size_t printprog;
    spinlock_t lock;
    streambuf_t stdin;
} stdinbuf_t;

decl_list( stdinbuf_t, stdinbufs );
stdinbuf_t* cur_stdinbuf;

void onkeypress( uint32_t scancode, char ASCII, os32_kb_action_t action )
{
    if (ASCII && action == KB_RELEASE)
    {
        streambuf_write( &cur_stdinbuf->stdin, &ASCII, 1 );
        if (ASCII == VGA_NEWLINE)
        {
            vfs_write( cur_proc->local_fdt[0], cur_stdinbuf->stdin.m_buf, cur_stdinbuf->stdin.m_size );
        }
    }    
    
}

void rtvgamain()
{
    vgaSetCursor( 0, 1 );
    vgaShowCursor(true);
    list_create( &stdinbufs );
    os32_keyboard_set_routine( onkeypress );

    pid_t pid = __spawn("/initrd/bin/sh", NULL, NULL);
    __yield();
    cur_proc = get_proc_by_id(pid);
    stdinbuf_t firststdbuf;
    firststdbuf.pid = pid;
    firststdbuf.printprog = 0;
    streambuf_create(&firststdbuf.stdin);
    list_push( &stdinbufs, &firststdbuf );
    cur_stdinbuf = list_reftp( list_end(&stdinbufs) );

    rtvga_termproc_t* first_proc = kmalloc(sizeof(rtvga_termproc_t));
    termproc_create( first_proc );
    first_proc->pid = pid;
    // for(;;);
    // memset(testerchar, 205, 80 );
    rtvga_cls( VGA_WHITE, VGA_BLACK );
    while(1)
    {
        rtvga_topbar_flip();
        cur_proc = get_proc_by_id((*rtvga_curproc)->pid);
        size_t new_data = 0;

        for (size_t i = 1; i < 3; i++)
        {
            char buffer[256] = {0};
            struct fstat stat;
            vfs_fstat( cur_proc->local_fdt[i], &stat );
            if (stat.size - vfs_tellg( cur_proc->local_fdt[i] ))
            {
                size_t amt = vfs_read( cur_proc->local_fdt[i], buffer, 256 );
                if (amt == OS32_ERROR)
                {
                    kpanic("rtvgamain");
                }
                if (amt == 0) continue;
                new_data+=amt;
                switch (i)
                {
                case 0:
                    // streambuf_write(&(*rtvga_curproc)->m_stdin, buffer, amt);
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
        }
        size_t amtin = cur_stdinbuf->stdin.m_size - cur_stdinbuf->printprog;
        
        if (amtin)
        {
            char inbuf[256] = {0};
            new_data += amtin;
            memcpy(inbuf, cur_stdinbuf->stdin.m_buf + cur_stdinbuf->printprog, amtin);
            streambuf_write(&(*rtvga_curproc)->m_stdin, inbuf, amtin);
            cur_stdinbuf->printprog+= amtin;
            if (inbuf[amtin-1] == VGA_NEWLINE)
            {
                __cli
                streambuf_destroy(&cur_stdinbuf->stdin);
                streambuf_create(&cur_stdinbuf->stdin);
                cur_stdinbuf->printprog = 0;
                __sti
            }
        }


        if (new_data)
        {
            termproc_display(*rtvga_curproc);
        }
    }

    for(;;);
}