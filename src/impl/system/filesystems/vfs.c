#include "system/filesystems/vfs.h"



static const struct vfs_fnode* first_fnode;
static const struct vfs_fnode* last_fnode;

int vfs_open( const char* fpath, int mode );
size_t vfs_write( fd_t fd, const char* data, size_t bytes );
size_t vfs_read( fd_t fd, char* dest, size_t bytes );
size_t vfs_seekg( fd_t fd, size_t amt, int whence );
size_t vfs_tellg( fd_t fd );
size_t vfs_seeko( fd_t fd, size_t amt, int whence );
size_t vfs_tello( fd_t fd );
err_t vfs_close( fd_t fd );
