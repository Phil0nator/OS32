#include "boot/vga.h"
#include "stdlib/string.h"
#include "stdlib/ioinstrs.h"
#include <stdbool.h>
#include <stdarg.h>

#define CRT_CONTROL_REGISTER_PORTNO_A 0x3D4
#define CRT_CONTROL_REGISTER_PORTNO_B 0x3D5
#define VGA_CURSOR_CRTINDEX_A 14
#define VGA_CURSOR_CRTINDEX_B 15

static struct {
    unsigned x;
    unsigned y;
} cursor;

static vgac_t cur_fg = VGA_WHITE;
static vgac_t cur_bg = VGA_BLACK;


VGACell_t* VGAO = (VGACell_t*)(0xc00b8000);


static void VGAScroll()
{
    for ( size_t i = 1; i < VGA_HEIGHT; i++)
    {
        memmove( &VGAO[ (i-1)*VGA_WIDTH ], &VGAO[ i*VGA_WIDTH ], VGA_WIDTH*sizeof(VGACell_t) );
    }
    memset( &VGAO[ (VGA_HEIGHT-1)*VGA_WIDTH ], 0, VGA_WIDTH * sizeof(VGACell_t) );
    cursor.y = VGA_HEIGHT-1;
}

static void VGAUpdateCursor()
{
    // http://www.osdever.net/bkerndev/Docs/whatsleft.htm

    uint16_t idx = cursor.x + cursor.y*VGA_WIDTH;

    outportb( CRT_CONTROL_REGISTER_PORTNO_A, VGA_CURSOR_CRTINDEX_A );
    outportb( CRT_CONTROL_REGISTER_PORTNO_B, idx >> 8 );
    outportb( CRT_CONTROL_REGISTER_PORTNO_A, VGA_CURSOR_CRTINDEX_B );
    outportb( CRT_CONTROL_REGISTER_PORTNO_B, idx );

}

static void VGAAdvance()
{
    cursor.x++;
    if (cursor.x >= VGA_WIDTH)
    {
        cursor.x = 0;
        if (cursor.y >= VGA_HEIGHT)
        {
            VGAScroll();
        }
        else
        {
            cursor.y++;
        }
    }
    VGAUpdateCursor();

}

void vgaNewline()
{
    cursor.x = 0;
    if (cursor.y >= VGA_HEIGHT)
    {
        VGAScroll();
    }
    else
    {
        cursor.y++;
    }
    VGAUpdateCursor();

}

void vgaPutc( char c )
{
    if (c == VGA_NEWLINE)
    {
        vgaNewline();
    }
    else if (c == VGA_CARRIAGE)
    {
        cursor.x = 0;
    }
    else
    {
        VGAO[ cursor.x + cursor.y*VGA_WIDTH ] = (VGACell_t){ c, cur_fg, cur_bg };
        VGAAdvance();
    }
}
void vgaPuts( const char* msg )
{
    while (*msg) vgaPutc( *(msg++) );
}
void vgaPutln( const char* msg )
{
    while (*msg) vgaPutc( *(msg++) );
    VGAScroll();
}

const char* vgaMultiBaseIntChars = "0123456789abcdef";

void vgaPrintInt( uint32_t num, bool sign, uint32_t base )
{
    
    char digits[22];
    
    if (num == 0){
        vgaPutc('0');
        return;
    } 
    else if (((int)num) < 0 && sign)
    {
        vgaPutc('-');
        num = -(int)num;
    }
    
    
    memset(digits, 0, sizeof(digits));
    size_t counter = 0;
    while (num)
    {
        digits[counter] = vgaMultiBaseIntChars[ ( num % base ) ];
        num /= base;
        counter ++;
    }
    do
    {
        counter--;
        vgaPutc(digits[counter]);
    }while (counter);
}

void vgaOk()
{
    vgac_t o_fg, o_bg;
    vgaGetCol(&o_fg, &o_bg);
    vgaSetCol( VGA_WHITE, VGA_BLACK );
    vgaPuts("[ ");
    vgaSetFg( VGA_GREEN );
    vgaPuts("OK");
    vgaSetFg( VGA_WHITE );
    vgaPuts(" ]");
    vgaSetCol(o_fg, o_bg);
}

void vgaFailed()
{
    vgac_t o_fg, o_bg;
    vgaGetCol(&o_fg, &o_bg);
    vgaSetCol( VGA_WHITE, VGA_BLACK );
    vgaPuts("[ ");
    vgaSetFg( VGA_RED );
    vgaPuts("FAILED");
    vgaSetFg( VGA_WHITE );
    vgaPuts(" ]");
    vgaSetCol(o_fg, o_bg);
}

void vgaPrintf( const char * format, ... )
{
    va_list args;
    va_start(args, format);
    
    while (*format)
    {
        if (*format == '%')
        {
            format++;
            switch (*format)
            {
            case 'd':
            case 'i':
                vgaPrintInt( va_arg(args, int), true, 10 );
                break;
            case 'x':
                vgaPrintInt( va_arg( args, int ), false, 16 );
                break;
            case 'u':
                vgaPrintInt( va_arg( args, int ), false, 10 );
                break;
            case 's':
                vgaPuts( va_arg(args, const char*) );
                break;
            case '%':
                vgaPutc('%');
                break;
            case 'c':
                vgaPutc( va_arg( args, int ) );
                break;
            case '+':
                vgaOk();
                break;
            case '-':
                vgaFailed();
                break;
            default:
                break;
            }
        }
        else
        {
            vgaPutc(*format);
        }
        format++;
    }
}
void vgaSetCol( vgac_t fg, vgac_t bg )
{
    cur_fg = fg;
    cur_bg = bg;
}
void vgaGetCol( vgac_t* fg, vgac_t* bg )
{
    *fg = cur_fg;
    *bg = cur_bg;
}
vgac_t vgaGetFg()
{
    return cur_fg;
}
vgac_t vgaGetBg()
{
    return cur_bg;
}
void vgaSetFg( vgac_t fg )
{
    cur_fg = fg;
}
void vgaSetBg( vgac_t bg )
{
    cur_bg = bg;
}

void vgaSetCursor( int x, int y )
{
    cursor.x = x;
    cursor.y = y;
    VGAUpdateCursor();
}
void vgaGetCursor( int* x, int* y )
{
    *x = cursor.x;
    *y = cursor.y;
}
void vgaShowCursor( bool show )
{
    
}