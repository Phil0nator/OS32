#ifndef OS32_TIMERDRIVER
#define OS32_TIMERDRIVER
#include <stdint.h>
#include <stddef.h>
#include "stdlib/error.h"

#define PIT_TPS ((float)18.222)
#define PIT_SECS( ticks ) ((ticks)/PIT_TPS)
#define PIT_TICKS( secs ) ((secs)*PIT_TPS)

typedef size_t pit_ticks_t;
typedef float pit_secs_t;


err_t __install_timer();

void pit_waitt( pit_ticks_t ticks );
void pit_waits( pit_secs_t seconds );
pit_ticks_t pit_ticks();

#endif