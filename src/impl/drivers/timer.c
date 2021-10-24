#include "drivers/timer.h"
#include "boot/irq.h"
#include "boot/vga.h"

//http://www.osdever.net/bkerndev/Docs/pit.htm

#define PIT_IRQNO (0)
// Ticks per second
#define PIT_TPS ((float)18.222)



static volatile pit_ticks_t total_ticks = 0;
// callback for each interrupt
static void timer_routine()
{
    total_ticks++;
}



void pit_waitt( pit_ticks_t ticks )
{
    pit_ticks_t final_ticks = total_ticks+ticks;
    while ( total_ticks < final_ticks );
}
void pit_waits( pit_secs_t seconds )
{
    pit_ticks_t start = total_ticks;
    while ( (total_ticks-start)/PIT_TPS < seconds );
}
pit_ticks_t pit_ticks()
{
    return total_ticks;
}

err_t __install_timer()
{
    irq_install_routine( PIT_IRQNO, (void*)timer_routine );
    return OS32_SUCCESS;
}
