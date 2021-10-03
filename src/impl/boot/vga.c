#include "boot/vga.h"
#include "stdlib/string.h"
#include <stdbool.h>
#include <stdarg.h>
static struct {
    unsigned x;
    unsigned y;
} cursor;

static vgac_t cur_fg = VGA_WHITE;
static vgac_t cur_bg = VGA_BLACK;

#pragma pack(1)

typedef struct VGACell
{
    char c;
    vgac_t fg:4;
    vgac_t bg:4;
} VGACell_t;

#pragma pack(0)

static VGACell_t* VGAO = (VGACell_t*)(0xc00b8000);

static void VGAScroll()
{
    for ( size_t i = 1; i < VGA_HEIGHT; i++)
    {
        memcpy( &VGAO[ (i-1)*VGA_WIDTH ], &VGAO[ i*VGA_WIDTH ], VGA_WIDTH*sizeof(VGACell_t) );
    }
    memset( &VGAO[ (VGA_HEIGHT-1)*VGA_WIDTH ], 0, VGA_WIDTH * sizeof(VGACell_t) );
}

static void VGAAdvance()
{
    cursor.x++;
    if (cursor.x >= VGA_WIDTH)
    {
        cursor.x = 0;
        cursor.y++;
        if (cursor.y >= VGA_HEIGHT)
        {
            VGAScroll();
        }
    }
}

void vgaNewline()
{
    cursor.x = 0;
    cursor.y++;
    if (cursor.y >= VGA_HEIGHT)
    {
        VGAScroll();
    }
}

void vgaPutc( char c )
{
    if (c == VGA_NEWLINE)
    {
        vgaNewline();
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

const char* vgaMultiBaseIntChars = "012345679abcdef";

void vgaPrintInt( unsigned num, bool sign, uint32_t base )
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
