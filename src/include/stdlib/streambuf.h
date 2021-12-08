#ifndef OS32_STREAMBUF
#define OS32_STREAMBUF
#include <stddef.h>
typedef int sbrd_mode_t;

#define SBRD_PASSIVE (0)
#define SBRD_CONSUME (1)

/**
 * @brief streambuf structs act as a way to dynamically
 * write to a buffer that can expand and contract with the data.
 */
typedef struct streambuf
{
    char* m_buf;
    size_t m_size;
} streambuf_t;

/**
 * @brief Initialize a streambuf
 * (won't call malloc yet)
 * @param dest pointer to target to initialize
 */
void streambuf_create( struct streambuf* dest );

/**
 * @brief write data to a streambuf 
 * 
 * @param dest streambuf to write to
 * @param data buffer to write
 * @param bytes number of bytes to write
 * @return size_t the number of bytes written (on success, should be the same as bytes)
 */
size_t streambuf_write( struct streambuf* dest, const char* data, size_t bytes );

/**
 * @brief read data from streambuf
 * 
 * the rmode parameter can be used to specify how data should be read.
 * - SBRD_PASSIVE will simply copy data into the buffer
 * - SBRD_CONSUME will copy the data into the buffer, and delete it from
 *  the streambuf
 * 
 * @param src streambuf
 * @param dest destination buffer
 * @param bytes number of bytes to read
 * @param rmode reading mode (see description)
 * @return size_t number of bytes read
 */
size_t streambuf_read( struct streambuf* src, char* dest, size_t bytes, sbrd_mode_t rmode );

/**
 * @brief free resources used by a streambuf
 * (if you want to use the same streambuf again,
 * you can call streambuf_create on it again)
 * @param dest streambuf to free
 */
void streambuf_destroy( struct streambuf* dest );

#endif