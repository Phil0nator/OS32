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

// maximum open files
#define VFS_MAX_OPEN 4096
// maximum mounted media
#define VFS_MAX_MEDIA 16
// maximum path length
#define VFS_MAX_PATH 256 

struct process; // forward decl

// first-time setup
err_t __install_vfs();

/**
 * @brief open a file
 * 
 * @param fpath file path
 * @param flags file flags
 * @param mode file mode
 * @return fd_t (-1 on error, sets errno) or positive file descriptor
 */
fd_t vfs_open( const char* fpath, int flags, int mode );

/**
 * @brief write to a file
 * 
 * @param fd file descriptor
 * @param data buffer to write
 * @param bytes number of bytes to write
 * @return size_t bytes written
 */
size_t vfs_write( fd_t fd, const char* data, size_t bytes );

/**
 * @brief read from a file
 * 
 * @param fd file descriptor
 * @param dest buffer to read into
 * @param bytes bytes to read
 * @return size_t bytes read
 */
size_t vfs_read( fd_t fd, char* dest, size_t bytes );

/**
 * @brief move the input position of a file descriptor
 * 
 * @param fd file descriptor
 * @param amt offset
 * @param whence origin to move from (0, current, end)
 * @return size_t success or error (sets errno on error)
 */
size_t vfs_seekg( fd_t fd, size_t amt, int whence );

/**
 * @brief get the current input position of a file descriptor
 * 
 * @param fd file descriptor
 * @return size_t input position
 */
size_t vfs_tellg( fd_t fd );

/**
 * @brief move the current output position of a file descriptor
 * 
 * @param fd file descriptor
 * @param amt offset
 * @param whence origin to move from (0, current, end)
 * @return size_t success or error (sets errno on error)
 */
size_t vfs_seeko( fd_t fd, size_t amt, int whence );

/**
 * @brief get the current output position of a file descriptor
 * 
 * @param fd file descriptor
 * @return size_t output position
 */
size_t vfs_tello( fd_t fd );

/**
 * @brief close a file descriptor
 * 
 * @param fd file descriptor
 * @return err_t success or error (sets errno on error)
 */
err_t vfs_close( fd_t fd );

/**
 * @brief perform setup for new process
 * 
 * creates the following:
 * - stdin streambuf
 * - stdout streambuf
 * - stderr streambuf
 * 
 * @param proc process
 * @return err_t success or error (sets errno on error)
 */
err_t vfs_setup_proc( struct process* proc );

/**
 * @brief clean resources used by a process
 * 
 * closes all file descriptors
 * 
 * @param proc process
 * @return err_t success or error (sets errno on error)
 */
err_t vfs_clean_proc( struct process* proc );

/**
 * @brief get stat info for file
 * 
 * @param path file path
 * @param buf stat struct
 * @return err_t success or error (sets errno on error)
 */
err_t vfs_stat(const char* path, struct fstat* buf);

/**
 * @brief get stat info for file by file descriptor
 * 
 * @param fd file descriptor
 * @param buf stat struct
 * @return err_t success or error (sets errno on error)
 */
err_t vfs_fstat(fd_t fd, struct fstat* buf);

/**
 * @brief get stat info for file link
 * 
 * @param path path
 * @param buf stat struct
 * @return err_t success or error (sets errno on error)
 */
err_t vfs_lstat(const char* path, struct fstat* buf);

/**
 * @brief get the path associated with a file descriptor
 * 
 * @param fd file descriptor
 * @return const char* statically allocated file path ( do not free )
 */
const char* vfs_fpath( fd_t fd );

#endif