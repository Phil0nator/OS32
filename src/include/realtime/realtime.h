#ifndef OS32_REALTIME
#define OS32_REALTIME

#ifdef OS32_RT_VESA

#else
#include "vga/rtvgamain.h"
#define rtmain rtvgamain
#endif

#include "krtloop.h"
#define kernel_mainloop krtloop

#endif