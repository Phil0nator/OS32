#include "realtime/vga/temulator.h"
#include "stdlib/ioinstrs.h"
#include "stdlib/string.h"


static void temu_consume( temu_t* t, const char* data, size_t bytes )
{
    for (size_t i = 0; i < bytes; i++)
    {
        
    }
}

void temu_create( temu_t* t )
{
    bzero((char*)t, sizeof(temu_t));
    streambuf_create( &t->vga_cells );
}
err_t temu_readfromsb( temu_t* t, streambuf_t* sb )
{
    temu_consume( t, sb->m_buf, sb->m_size );
    streambuf_destroy(sb);
    streambuf_create(sb);
}
err_t temu_readfroms( temu_t* t, const char* str )
{
    temu_consume( t, str, strlen(str) );
}
err_t temu_apply( temu_t* t )
{

}
void temu_destroy( temu_t* t )
{
    streambuf_destroy( &t->vga_cells );
}
