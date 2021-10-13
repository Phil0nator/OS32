#include "realtime/vga/rtvgamain.h"
#include "realtime/vga/utils.h"
#include "drivers/rtc.h"
#include "realtime/vga/topbar.h"
#include "drivers/timer.h"


void rtvgamain()
{
    while(1)
    {
        rtvga_topbar_flip();

        rtvga_cls( VGA_BLACK );
        vgaSetBg( VGA_LIGHT_GRAY );
        vgaSetFg( VGA_WHITE );
        vgaSetCursor(0,0);
        vgaPuts( rtvga_topbar );
        pit_waitt(5);
    }

    for(;;);
}