#include "stdlib/streambuf.h"
#include "stdlib/kmalloc.h"
#include "stdlib/string.h"
void streambuf_create( struct streambuf* dest )
{
    dest->m_buf = NULL;
    dest->m_size = 0;
}
size_t streambuf_write( struct streambuf* dest, const char* data, size_t bytes )
{
    size_t newsize = dest->m_size + bytes;
    dest->m_buf = krealloc( dest->m_buf, newsize );
    memcpy( &dest->m_buf[dest->m_size], data, bytes );
    dest->m_size = newsize;
    return bytes;
}
size_t streambuf_read( struct streambuf* src, char* dest, size_t bytes, sbrd_mode_t rmode )
{
    size_t read_amount = (src->m_size > bytes) ? (bytes) : (src->m_size);
    memcpy(dest, src->m_buf, read_amount);
    if (rmode == SBRD_CONSUME)
    {
        src->m_buf = krealloc( src->m_buf, src->m_size - read_amount );
        src->m_size -= read_amount;
    }
    return read_amount;
}
void streambuf_destroy( struct streambuf* dest )
{
    kfree(dest->m_buf);
    dest->m_buf = NULL;
    dest->m_size = 0;
}
