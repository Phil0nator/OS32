#ifndef OS32_RTVGA_TERMPROC
#define OS32_RTVGA_TERMPROC
#include "boot/vga.h"
#include "stdlib/streambuf.h"
#include "realtime/vga/temulator.h"
#include "system/process/process.h"
#define RTVGA_TERMWIDTH 79
#define RTVGA_TERMHEIGHT 24

typedef struct rtvga_termproc
{
    streambuf_t m_stdin;
    streambuf_t m_stdout;
    streambuf_t m_stderr;
    pid_t pid;
    size_t scrolly;
    temu_t te;


} rtvga_termproc_t;

void termproc_create( rtvga_termproc_t* tp );
void termproc_destroy( rtvga_termproc_t* tp );
void termproc_display( rtvga_termproc_t* tp );


extern rtvga_termproc_t** rtvga_curproc;


#endif