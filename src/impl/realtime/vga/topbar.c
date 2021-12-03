#include "realtime/vga/topbar.h"
#include "boot/vga.h"
#include "drivers/rtc.h"
#include "stdlib/string.h"
#include "stdlib/format.h"


char rtvga_topbar[VGA_WIDTH+1] = {0};


void rtvga_topbar_flip()
{
    int cx, cy;
    vgaGetCursor(&cx, &cy);
    rtc_timepoint_t tm;
    char prev_topbar[VGA_WIDTH];
    memcpy(prev_topbar, rtvga_topbar, VGA_WIDTH);

    rtc_get_time(&tm);
    // |MMM DD HH:MM
    char date[13] = {'\0'};
    // process left
    // |<< NN/NN
    // char proc_left[8] = {'\0'};
    // process right
    // |>> NN/NN
    // char proc_right[8] = {'\0'};

    snprintf( 
        date, 
        sizeof(date), 
        "%s %d %t:%t", 
        rtc_month_names[tm.m_month-1],
        tm.m_mday,
        tm.m_hour,
        tm.m_min );
    
    memset( rtvga_topbar, ' ', VGA_WIDTH );
    memset( rtvga_topbar, '<', 2 );
    memset( rtvga_topbar+VGA_WIDTH-2, '>', 2 );
    memcpy( (rtvga_topbar+VGA_WIDTH/2)-((strlen(date))/2), date, 11 );
    

    if (!memequ( prev_topbar, rtvga_topbar, VGA_WIDTH ))
    {
        vgaSetBg( VGA_LIGHT_GRAY );
        vgaSetFg( VGA_WHITE );
        vgaSetCursor(0,0);
        vgaPuts( rtvga_topbar );
        vgaSetCol( VGA_WHITE, VGA_BLACK );
        vgaSetCursor( cx, cy );
    }


}