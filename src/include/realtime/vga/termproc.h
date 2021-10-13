#ifndef OS32_RTVGA_TERMPROC
#define OS32_RTVGA_TERMPROC
#include "boot/vga.h"
#include "stdlib/streambuf.h"
#define RTVGA_TERMWIDTH 80
#define RTVGA_TERMHEIGHT 23

typedef struct rtvga_termproc
{
    streambuf_t m_stdin;
    streambuf_t m_stdout;
    streambuf_t m_stderr;
    __pid_t pid;

} rtvga_termproc_t;

#endif