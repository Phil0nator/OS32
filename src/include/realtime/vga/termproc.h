#ifndef OS32_RTVGA_TERMPROC
#define OS32_RTVGA_TERMPROC
#include "boot/vga.h"
#include "stdlib/streambuf.h"
#define RTVGA_TERMWIDTH 79
#define RTVGA_TERMHEIGHT 24

typedef struct rtvga_termproc
{
    streambuf_t m_stdin;
    streambuf_t m_stdout;
    streambuf_t m_stderr;
    streambuf_t m_rawtext;
    __pid_t pid;
    size_t scrolly;

} rtvga_termproc_t;

extern rtvga_termproc_t* rtvga_curproc;

#endif