#ifndef OS32_STREAMBUF
#define OS32_STREAMBUF
#include <stddef.h>
typedef int sbrd_mode_t;

#define SBRD_PASSIVE (0)
#define SBRD_CONSUME (1)


typedef struct streambuf
{
    char* m_buf;
    size_t m_size;
} streambuf_t;

void streambuf_create( struct streambuf* dest );
size_t streambuf_write( struct streambuf* dest, const char* data, size_t bytes );
size_t streambuf_read( struct streambuf* src, char* dest, size_t bytes, sbrd_mode_t rmode );
void streambuf_destroy( struct streambuf* dest );

#endif