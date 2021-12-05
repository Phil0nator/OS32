#include "realtime/vga/temulator.h"
#include "stdlib/ioinstrs.h"
#include "stdlib/string.h"
#include "stdlib/ANSI.h"
#include "boot/vga.h"
#include "stdlib/kmalloc.h"

static void temu_consume( temu_t* t, const char* data, size_t bytes )
{
    for (size_t i = 0; i < bytes; i++)
    {
        if ( data[i] == ANSI_ESC )
        {

        }
        else if (data[i] == '\n')
        {  
            vgac_t oldfg, oldbg;
            oldfg = t->state.fg;
            oldbg = t->state.bg;
            t->state.fg = VGA_BLACK;
            t->state.bg = VGA_BLACK;
            while (t->vga_cells.m_size % VGA_WIDTH != 0)
            {
                temu_consume( t, " ", 1 );
            }
            t->state.fg = oldfg;
            t->state.bg = oldbg;
            t->state.cursor.x = 0;
            t->state.cursor.y++;
        }
        else
        {
            VGACell_t cell;
            cell.c = data[i];
            cell.fg = t->state.fg;
            cell.bg = t->state.bg;
            if (t->state.cursor.x + t->state.cursor.y*VGA_WIDTH == t->vga_cells.m_size/sizeof(cell))
            {
                streambuf_write( &t->vga_cells, &cell, sizeof(cell) );
            }
            else
            {
                *(VGACell_t*)(&t->vga_cells.m_buf[(t->state.cursor.x + t->state.cursor.y*VGA_WIDTH)*sizeof(cell)]) = cell;
            }
            t->state.cursor.x++;
            if (t->state.cursor.x >= VGA_WIDTH)
            {
                t->state.cursor.y++;
                t->state.cursor.x=0;
            }
        }
    }
}

void temu_create( temu_t* t )
{
    bzero((char*)t, sizeof(temu_t));
    streambuf_create( &t->vga_cells );
    t->vga_cells.m_buf = krealloc( t->vga_cells.m_buf, (VGA_WIDTH*(VGA_HEIGHT-1))*sizeof(VGACell_t) );
    t->vga_cells.m_size =  (VGA_WIDTH*(VGA_HEIGHT-1))*sizeof(VGACell_t);
    for (size_t i = 0; i < (VGA_WIDTH*(VGA_HEIGHT-1)); i++ ) 
    {
        *(VGACell_t*)(&t->vga_cells.m_buf[i*sizeof(VGACell_t)]) = (VGACell_t){.fg=VGA_WHITE, .bg=VGA_BLACK,.c=' '};
    }
    t->state.fg=VGA_WHITE;
    t->state.bg=VGA_BLACK;
}
err_t temu_readfromsb( temu_t* t, streambuf_t* sb, bool consume )
{
    temu_consume( t, sb->m_buf, sb->m_size );
    if (consume)
    {
        streambuf_destroy(sb);
        streambuf_create(sb);
    }
}
err_t temu_readfroms( temu_t* t, const char* str )
{
    temu_consume( t, str, strlen(str) );
}
err_t temu_apply( temu_t* t )
{
    memcpy( VGAO+VGA_WIDTH, t->vga_cells.m_buf + t->state.scroll * VGA_WIDTH, VGA_WIDTH*(VGA_HEIGHT-1) );
    vgaSetCursor( t->state.cursor.x, t->state.cursor.y+1-t->state.scroll );
}
void temu_destroy( temu_t* t )
{
    streambuf_destroy( &t->vga_cells );
}
