#include "drivers/timer.h"
#include "boot/irq.h"
#include "boot/vga.h"
#include "system/process/multitasking.h"
#include "stdlib/ioinstrs.h"

//http://www.osdever.net/bkerndev/Docs/pit.htm

#define PIT_IRQNO (0)
#define PIT_DIVISOR (1193180)
#define PIT_REG_CMD (0x43)
#define PIT_REG_CH0 (0x40)
#define PIT_REG_CH1 (0x41)
#define PIT_REG_CH2 (0x42)

#define PIT_MODE_ITTC (0 << 1)
#define PIT_MODE_RETRIG (1 << 1)
#define PIT_MODE_RATE (2 << 1)
#define PIT_MODE_SQUARE (3 << 1)
#define PIT_MODE_SOFTSTROBE (4 << 1)
#define PIT_MODE_HARDSTROBE (5 << 1)

#define PIT_RW_LSB (1 << 4)
#define PIT_RW_MSB (2 << 4)
#define PIT_RW_LSB_MSB (3 << 4)

#define PIT_BCD_ON 1
#define PIT_BCD_OFF 0

#define PIT_CONTROL_BIT( bcd, mode, rw, cntr ) ( (bcd) | (mode) | (rw) | (cntr))

// Ticks per second
// #define PIT_TPS ((float)18.222)



static volatile pit_ticks_t total_ticks = 0;
// callback for each interrupt
static void timer_routine()
{
    total_ticks++;

    if (!current_process)
    {
        return; // if multitasking is not yet installed, move on
    }
    current_process->quantum_progress ++;
    if (current_process->quantum_progress >= current_process->quantum)
    {
        __procswitch();
    }
}

void pit_sethz( size_t hz )
{

    int divided = hz/PIT_DIVISOR;
    outportb( PIT_REG_CMD, PIT_CONTROL_BIT( PIT_BCD_OFF, PIT_MODE_SQUARE, PIT_RW_LSB_MSB, 0 ) );
    outportb( PIT_REG_CH0, divided & 0xff );
    outportb( PIT_REG_CH0, divided >> 8 );
}

void pit_waitt( pit_ticks_t ticks )
{
    volatile pit_ticks_t final_ticks = total_ticks+ticks;
    while ( total_ticks < final_ticks );
}
void pit_waits( pit_secs_t seconds )
{
    volatile pit_ticks_t start = total_ticks;
    while ( (total_ticks-start)/PIT_TPS < seconds );
}
pit_ticks_t pit_ticks()
{
    return total_ticks;
}

err_t __install_timer()
{
    irq_install_routine( PIT_IRQNO, (void*)timer_routine );
    pit_sethz( PIT_TPS );
    return OS32_SUCCESS;
}
