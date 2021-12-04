#include "realtime/vga/termproc.h"
#include "stdlib/string.h"


rtvga_termproc_t* rtvga_proclist[256] = {0};
rtvga_termproc_t** rtvga_curproc = &rtvga_proclist[0];


void termproc_create( rtvga_termproc_t* tp )
{
    temu_create(&tp->te);
    streambuf_create(&tp->m_stdout);
    streambuf_create(&tp->m_stdin);
    streambuf_create(&tp->m_stderr);
    tp->scrolly = 0;
    tp->pid = 0;

    for (size_t i = 0; i < 256; i++)
    {
        if (!rtvga_proclist[i])
        {
            rtvga_proclist[i] = tp;
            return;
        }
    }
    
}
void termproc_destroy( rtvga_termproc_t* tp )
{
    streambuf_destroy(&tp->m_stdout);
    streambuf_destroy(&tp->m_stdin);
    streambuf_destroy(&tp->m_stderr);
    temu_destroy(&tp->te);
    for (size_t i = 0; i < 256; i++)
    {
        if (rtvga_proclist[i] == tp)
        {
            rtvga_proclist[i] = NULL;
            return;
        }
    }
}
void termproc_display( rtvga_termproc_t* tp )
{
    temu_readfromsb( &tp->te, &tp->m_stdout, true );
    temu_readfromsb( &tp->te, &tp->m_stderr, true );
    temu_readfromsb( &tp->te, &tp->m_stdin, true );
    temu_apply( &tp->te );
}
