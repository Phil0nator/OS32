#ifndef OS32_EXT2
#define OS32_EXT2
#include "system/filesystems/types.h"
#include "stdlib/error.h"
#include "stdlib/time.h"

#define EXT2_MAX_OPEN 512
#define EXT2_SIGNATURE (0xef53)

//https://wiki.osdev.org/Ext2
typedef uint16_t ext2_uid;
typedef uint16_t ext2_gid;

// Opaque type used to represent an ext2 partition
struct ext2_partition;

/**
 * @brief ext2-specific stat structure
 */
struct ext2_fstat
{
    // device
    dev_t dev;
    // inode number
    uint32_t inode;
    // mode
    uint32_t mode;
    // number of links
    uint32_t nlink;
    // user id
    ext2_uid uid;
    // group id
    ext2_gid gid;
    // size in bytes
    size_t size;
    // access time
    pxtime_t atime;
    // modify time
    pxtime_t mtime;
    // create time
    pxtime_t ctime;
};

/**
 * @brief initialize an ext2 partition based on the raw data    
 * 
 * the pointer scribbled by this function must be free'd with
 * ext2_free
 * 
 * @param dest pointer to ext2_partition pointer 
 * @param raw raw data pointer
 * @return err_t either success or error. (errno will be set on error)
 */
err_t ext2_init( struct ext2_partition** dest, char* raw );

/**
 * @brief free resources used by ext2 partition
 * ( this will only free the raw data if the parameter is set to true)
 * @param p partition to free
 * @param freeRaw wether or not to free raw data
 */
void ext2_free( struct ext2_partition* p, bool freeRaw );

/**
 * @brief get raw data pointer
 * 
 * @param p partition
 * @return char* raw data pointer held by p
 */
char* ext2_get_raw( struct ext2_partition* p );

/**
 * @brief open a file in an ext2 partition
 * 
 * @param p partition
 * @param fname filepath
 * @return fd_t (-1 on error. sets errno), or positive file descriptor. 
 */
fd_t ext2_open( struct ext2_partition* p, const char* fname );

/**
 * @brief write to a file in an ext2 partition
 * 
 * @param p ext2 partition
 * @param fd file descriptor
 * @param data buffer to write 
 * @param bytes size of buffer to write
 * @param start position in file to write to
 * @return size_t bytes written
 */
size_t ext2_write(struct ext2_partition* p, fd_t fd, const char* data, size_t bytes, size_t start );

/**
 * @brief read from a file in an ext2 partition
 * 
 * @param p ext2 partition
 * @param fd file descriptor
 * @param dest destination buffer
 * @param bytes bytes to read
 * @param start position in file to read from
 * @return size_t bytes read
 */
size_t ext2_read(struct ext2_partition* p, fd_t fd, char* dest, size_t bytes, size_t start );

/**
 * @brief close a file descriptor for an ext2 partition
 * 
 * @param p partition
 * @param fd file descriptor
 * @return err_t success or error (set errno on failure)
 */
err_t ext2_close(struct ext2_partition* p, fd_t fd );

/**
 * @brief get stat info for a file in an ext2 partition
 * 
 * @param p ext2 partition
 * @param path path to file
 * @param buf ext2_fstat struct to write to
 * @return err_t success or error (sets errno on error)
 */
err_t ext2_stat(struct ext2_partition* p, const char* path, struct ext2_fstat* buf);

/**
 * @brief get stat info for a file in an ext2 partition from a file descriptor
 * 
 * @param p ext2 partition
 * @param fd file descriptor
 * @param buf ext2_fstat struct to write to
 * @return err_t success or error (sets errno on error)
 */
err_t ext2_fstat(struct ext2_partition* p, fd_t fd, struct ext2_fstat* buf);

/**
 * @brief get stat info for a link file in an ext2 partition
 * 
 * @param p ext2 partition
 * @param path path to link file
 * @param buf ext2_fstat struct to write to
 * @return err_t success or error (sets errno on error)
 */
err_t ext2_lstat(struct ext2_partition* p, const char* path, struct ext2_fstat* buf);

/**
 * @brief make a directory in an ext2 partition
 * 
 * @param p ext2 partition
 * @param path path to new directory
 * @param mode file mode
 * @return err_t success or error (sets errno on error)
 */
err_t ext2_mkdir( struct ext2_partition* p, const char* path, mode_t mode );

#endif