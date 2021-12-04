#include "drivers/keyboard.h"
#include "boot/irq.h"
#include "stdlib/ioinstrs.h"
#include "boot/vga.h"
#include "stdlib/assert.h"

//http://www.osdever.net/bkerndev/Docs/keyboard.htm

#define KEYBOARD_IRQNO 1
#define KEYBOARD_PORTNO 0x60
#define KB_RELEASE_BIT_SET( x ) ( (x) & 0x80 )

static const unsigned char kb_en_us[] = 
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};		

static const unsigned char* kb_layouts[] = 
{
    [KB_EN_US] = kb_en_us
};


static os32_kb_routine cur_subroutine = NULL;
static os32_kb_layout_t cur_layout = KB_EN_US;

static void keyboard_routine()
{
    uint8_t scancode = inportb( KEYBOARD_PORTNO );
    os32_kb_action_t action;
    if ( KB_RELEASE_BIT_SET(scancode) )
    {
        action = KB_RELEASE;
        scancode &= ~0x80;
    }
    else
    {
        action = KB_PRESS;
    }
    if (cur_subroutine)
    {
        if (action == KB_RELEASE) 
            vga_assert(action);
        cur_subroutine( scancode, kb_layouts[cur_layout][scancode], action );
    }
}


err_t __install_keyboard()
{
    irq_install_routine( KEYBOARD_IRQNO, (void*)keyboard_routine );
    return OS32_SUCCESS;
}


void os32_keyboard_set_routine( os32_kb_routine routine )
{
    cur_subroutine = routine;
}
void os32_keyboard_set_layout( os32_kb_layout_t layout )
{
    cur_layout = layout;
}
