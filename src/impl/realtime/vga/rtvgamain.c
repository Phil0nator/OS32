#include "realtime/vga/rtvgamain.h"
#include "realtime/vga/utils.h"
#include "drivers/rtc.h"
#include "realtime/vga/topbar.h"
#include "drivers/timer.h"
#include "stdlib/string.h"

static const char testerchar[81] = {'\0'};

void rtvgamain()
{
    // memset(testerchar, 205, 80 );
    rtvga_cls( VGA_BLACK );
    while(1)
    {
        rtvga_topbar_flip();
        
        
    }

    for(;;);
}