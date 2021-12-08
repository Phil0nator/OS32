#ifndef OS32_TIMERDRIVER
#define OS32_TIMERDRIVER
#include <stdint.h>
#include <stddef.h>
#include "stdlib/error.h"

#define PIT_DEFAULT_TPS ((float)18.222)

/**
 * PIT devices Ticks per Second
 */
#define PIT_TPS             ((float)1000)
/**
 * Convert PIT ticks to seconds
 */
#define PIT_SECS( ticks )   ((ticks)/PIT_TPS)
/**
 * Convert seconds to PIT ticks
 */
#define PIT_TICKS( secs )   ((secs)*PIT_TPS)

typedef size_t pit_ticks_t;
typedef float pit_secs_t;

/**
 * Perform timer installation
 */
err_t __install_timer();

/**
 * Wait a certain number of ticks
 */
void pit_waitt( pit_ticks_t ticks );
/**
 * Wait a certain number of seconds
 */
void pit_waits( pit_secs_t seconds );

/**
 * Get the current tick count
 */
pit_ticks_t pit_ticks();

#endif