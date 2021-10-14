#ifndef OS32_VFS
#define OS32_VFS
#include <stdint.h>
#include <stddef.h>
#include "types.h"
#include "stdlib/error.h"

#define VFS_WRITE 1
#define VFS_CREAT 2
#define VFS_APPEN 4

#define VFS_SEEK_SET   0
#define VFS_SEEK_CUR   1 
#define VFS_SEEK_END   2

int vfs_open( const char* fpath, int mode );
size_t vfs_write( fd_t fd, const char* data, size_t bytes );
size_t vfs_read( fd_t fd, char* dest, size_t bytes );
size_t vfs_seekg( fd_t fd, size_t amt, int whence );
size_t vfs_tellg( fd_t fd );
size_t vfs_seeko( fd_t fd, size_t amt, int whence );
size_t vfs_tello( fd_t fd );
err_t vfs_close( fd_t fd );


#endif