#ifndef OS32_STRING
#define OS32_STRING
#include <stddef.h>
#include <stdbool.h>
#include <stdlib/error.h>

/**
 * Copy non-overlapping memory
 * @param dest destination of copy
 * @param source source of copy
 * @param bytes bytes to copy
 */
void memcpy( void * dest, const void * source, size_t bytes );
/**
 * Set a chunk of memory to a given byte value
 * @param dest memory start
 * @param byte value to set
 * @param count number of bytes of dest to set
 */
void memset( void * dest, char byte, size_t count );
/**
 * Move memory from one address to another
 * ( Will work for overlapping memory )
 * @param dest destination
 * @param source original location
 * @param bytes number of bytes to move
 */
void memmove( void* dest, const void* source, size_t bytes );
/**
 * Zero out a chunk of memory
 * @param dest memory start
 * @param bytes number of bytes to zero
 */
void bzero( void* dest, size_t bytes );
/**
 * Compare two blocks of memory
 * @param a one block
 * @param b another block
 * @param bytes number of bytes to compare
 */
int memcmp( const char* a, const char* b, size_t bytes );
/**
 * Compare if two blocks of memory are equal
 * (1 = true, 0 = false)
 * @param a one block
 * @param b another block
 * @param bytes number of bytes to compare
 */
int memequ( const char* a, const char* b, size_t bytes );

/**
 * Copy a null-terminated string
 * @param dest destination
 * @param src source
 */
size_t strcpy( char* dest, const char* src );
/**
 * Copy a null-terminated string up to n bytes
 * @param dest destination
 * @param n max bytes to copy
 * @param src source
 */
size_t strncpy( char* dest, size_t n, const char* src );
/**
 * Determine the length of a null terminated string
 * @param data cstring
 */
size_t strlen( const char* data );
/**
 * Find the first occurence of a given character in a cstring
 * ( -1 if not present )
 * @param str a cstring
 * @param chr the char to look for
 */
size_t strchr( const char* str, char chr );
/**
 * Compare if two cstrings are equal
 * @param a cstring 1
 * @param b cstring 2
 */
bool strequ( const char* a, const char* b );
/**
 * Get the description string of a given error number
 */
const char* strerror( errno_t e );
/**
 * Determine if one cstring begins with another
 * i.e:
 *      a = "one two three four"
 *      b = "one"
 *      return value: true
 * 
 * @param a cstring 1
 * @param b cstring 2
 */
bool starts_with( const char* a, const char* b );

#endif