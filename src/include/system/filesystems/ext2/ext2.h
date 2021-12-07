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

struct ext2_partition;
struct ext2_fstat
{
    dev_t dev;
    uint32_t inode;
    uint32_t mode;
    uint32_t nlink;
    ext2_uid uid;
    ext2_gid gid;
    size_t size;
    pxtime_t atime;
    pxtime_t mtime;
    pxtime_t ctime;
};

err_t ext2_init( struct ext2_partition** dest, char* raw );
void ext2_free( struct ext2_partition* p, bool freeRaw );
char* ext2_get_raw( struct ext2_partition* p );

fd_t ext2_open( struct ext2_partition* p, const char* fname );
size_t ext2_write(struct ext2_partition* p, fd_t fd, const char* data, size_t bytes, size_t start );
size_t ext2_read(struct ext2_partition* p, fd_t fd, char* dest, size_t bytes, size_t start );
err_t ext2_close(struct ext2_partition* p, fd_t fd );

err_t ext2_stat(struct ext2_partition* p, const char* path, struct ext2_fstat* buf);
err_t ext2_fstat(struct ext2_partition* p, fd_t fd, struct ext2_fstat* buf);
err_t ext2_lstat(struct ext2_partition* p, const char* path, struct ext2_fstat* buf);

err_t ext2_mkdir( struct ext2_partition* p, const char* path, mode_t mode );

#endif