#include "drivers/keyboard.h"
#include "boot/irq.h"
#include "stdlib/ioinstrs.h"
#include "boot/vga.h"

//http://www.osdever.net/bkerndev/Docs/keyboard.htm

#define KEYBOARD_IRQNO 1
#define KEYBOARD_PORTNO 0x60
#define KB_RELEASE_BIT_SET( x ) ( (x) & 0x80 )

static os32_kb_routine cur_subroutine = NULL;
static os32_kb_layout_t cur_layout = KB_EN_US;

static void keyboard_routine()
{
    uint8_t scancode = inportb( KEYBOARD_PORTNO );
    os32_kb_action_t action;
    if ( KB_RELEASE_BIT_SET(scancode) )
    {
        action = KB_RELEASE;
    }
    else
    {
        action = KB_PRESS;
    }
    if (cur_subroutine)
    {
        cur_subroutine( scancode, ' ', action );
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
