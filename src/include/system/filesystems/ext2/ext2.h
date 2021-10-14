#ifndef OS32_EXT2
#define OS32_EXT2
#include "system/filesystems/types.h"
#include "stdlib/error.h"

//https://wiki.osdev.org/Ext2

struct ext2_partition;

void ext2_init( struct ext2_partition* dest, char* raw );
void ext2_free( struct ext2_partition* p, bool freeRaw );
char* ext2_get_raw( struct ext2_partition* p );

fd_t ext2_open( struct ext2_partition* p, const char* fname );
size_t ext2_write(struct ext2_partition* p, fd_t fd, const char* data, size_t bytes );
size_t ext2_read(struct ext2_partition* p, fd_t fd, char* dest, size_t bytes );
size_t ext2_seekg(struct ext2_partition* p, fd_t fd, size_t amt, int whence );
size_t ext2_tellg(struct ext2_partition* p, fd_t fd );
size_t ext2_seeko(struct ext2_partition* p, fd_t fd, size_t amt, int whence );
size_t ext2_tello(struct ext2_partition* p, fd_t fd );
err_t ext2_close(struct ext2_partition* p, fd_t fd );


#endif