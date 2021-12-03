#ifndef OS32_RTVGA_TEMULATOR
#define OS32_RTVGA_TEMULATOR
#include "boot/vga.h"
#include "stdlib/streambuf.h"
#include "stdlib/error.h"

typedef struct temu
{
    streambuf_t vga_cells;
    struct 
    {
        struct 
        {
            uint32_t x, y;
        } cursor;
        
        vgac_t fg, bg;
        uint32_t scroll;
    } state;
} temu_t;

void temu_create( temu_t* t );
err_t temu_readfromsb( temu_t* t, streambuf_t* sb, bool consume );
err_t temu_readfroms( temu_t* t, const char* str );
err_t temu_apply( temu_t* t );
void temu_destroy( temu_t* t );


#endif