#include "realtime/vga/utils.h"
#include <stddef.h>

void rtvga_cls( vgac_t fg, vgac_t bg )
{
    VGACell_t cell = (struct VGACell){ 
        ' ',
        fg,
        bg
    };
    for ( size_t x = 0; x < VGA_WIDTH; x++ )
    {
        for (size_t y = 0; y < VGA_HEIGHT; y++)
        {
            VGAO[ x+y*VGA_WIDTH ] = cell;
        }
    }
}
