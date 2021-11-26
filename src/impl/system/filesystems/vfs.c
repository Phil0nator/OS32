#include "system/process/process.h"
#include "system/filesystems/vfs.h"
#include "stdlib/kmalloc.h"
#include "stdlib/string.h"
#include "system/filesystems/linitrd.h"
#include "stdlib/streambuf.h"

#define VFS_ASSERT_FD(fd) if (fd < 0){__set_errno(EBADF);return OS32_ERROR;}

typedef int fd_type;
#define FD_FILE 0
#define FD_STDSTREAM 1
#define FD_SOCK 2

typedef int fs_type;
#define FD_EXT2 0

#define VFS_FS_TYPES (1)

/**
 * fd_pos represents a file descriptors cursor in a file.
 * i.e its input reading position and output writing position.
 * ( modifiable via seekg/seeko )
 */
typedef struct fd_pos
{
    size_t ipos;
    size_t opos;
} fd_pos_t;
/**
 * fd_entry represents an entry in the file descriptor table
 */
typedef struct fd_entry
{
    // Does this entry exist
    bool present;
    // File descriptor type
    fd_type fdtp;
    // Depending on the type of file descriptor,
    // one of the following structs...
    union
    {
        // for files,
        struct fd_file
        {
            // filesystem type
            fs_type fstp;
            // descriptor to the mounted partition
            fd_t partition_mount;
            // descriptor provided by the underlying implimentation for that fs
            fd_t underlying_fd;
        } file;
        streambuf_t sb;
    };
    // cursor
    fd_pos_t pos;
    
} fd_entry_t;

/**
 * vfs_partition represents a partition of any filesystem
 * that has been loaded to the vfs
 */
typedef struct vfs_partition
{
    // does this entry exist
    bool present;
    // filesystem type
    fs_type fstp;
    // file descriptor type
    fd_type fdtp;
    // void pointer to underlying implimentation struct
    void* underlying;
    // path where this partition is mounted
    char mountpoint[VFS_MAX_PATH]; 
} vfs_partition_t;

/**
 * fstp_methods, and its accompanying typedefs, represent
 * the standard filesystem operation functions provided by
 * any lower level implimentation of a filesystem.
 */

typedef fd_t (*fstp_m_open)( void* p, const char* f );
typedef size_t (*fstp_m_read)( void* p, fd_t fd, char* dest, size_t bytes, size_t start );
typedef size_t (*fstp_m_write)( void* p, fd_t fd, const char* src, size_t bytes, size_t start );
typedef err_t (*fstp_m_close)( void* p, fd_t fd );

typedef err_t (*fstp_m_stat)(void* p, const char* path, struct ext2_fstat* buf);
typedef err_t (*fstp_m_fstat)(void* p, fd_t fd, struct ext2_fstat* buf);
typedef err_t (*fstp_m_lstat)(void* p, const char* path, struct ext2_fstat* buf);

typedef struct fstp_methods
{
    const fstp_m_open m_open; 
    const fstp_m_read m_read; 
    const fstp_m_write m_write; 
    const fstp_m_close m_close; 
    const fstp_m_stat m_stat;
    const fstp_m_fstat m_fstat;
    const fstp_m_lstat m_lstat;
} fstp_methods_t;

static const fstp_methods_t fs_methods[VFS_FS_TYPES] =
{
    {
        (fstp_m_open)ext2_open,
        (fstp_m_read)ext2_read,
        (fstp_m_write)ext2_write,
        (fstp_m_close)ext2_close,
        (fstp_m_stat)ext2_stat,
        (fstp_m_fstat)ext2_fstat,
        (fstp_m_lstat)ext2_lstat
    }
};

// File Descriptor Table
static fd_entry_t fdt[VFS_MAX_OPEN];
// Virtual File System Partition Descriptor Table
static vfs_partition_t vfspdt[VFS_MAX_MEDIA];


size_t vfs_read_file( fd_t fd, char* dest, size_t bytes );
size_t vfs_write_file( fd_t fd, const char* src, size_t bytes );
err_t vfs_close_file( fd_t fd );


// Get the next available descriptor 
fd_t alloc_next_fd()
{
    for (fd_t i = 3; i < VFS_MAX_OPEN; i++)
    {
        if (!fdt[i].present)
        {
            fdt[i].present = true;
            return i;
        }
    }
    return OS32_ERROR;
}
// Allocate a new partition descriptor
err_t alloc_mount_partition( fd_type fdtp, fs_type t, void* u, char* m )
{
    for (size_t i = 0; i < VFS_MAX_MEDIA; i++)
    {
        if (!vfspdt[i].present)
        {
            vfspdt[i].present = true;
            vfspdt[i].fstp = t;
            vfspdt[i].fdtp = fdtp;
            vfspdt[i].underlying = u;
            strcpy( vfspdt[i].mountpoint, m );
            return i;
        }
    }
    return OS32_ERROR;
}


err_t __install_vfs()
{
    // mount the initrd
    alloc_mount_partition(FD_FILE, FD_EXT2, __initrd, "/initrd" );
    return OS32_SUCCESS;
}


fd_t vfs_open( const char* fpath, int mode )
{
    // Search through all mounted partitions
    for (size_t i = 0; i < VFS_MAX_MEDIA; i++ )
    {
        // check if this path goes to this mounted partition
        if ( starts_with( fpath, vfspdt[i].mountpoint) )
        {
            // fspath is the path without the partition part.
            // i.e: /initrd/bin/bash
            //      - fspath would = "/bin/bash"
            const char* fspath = fpath + strlen(vfspdt[i].mountpoint) -1;
            // Use the correct open() method for the given fs
            fd_t underlying = fs_methods[vfspdt[i].fstp].m_open
            (
                vfspdt[i].underlying,
                fspath
            );

            if (underlying < 0)
            {
                return OS32_ERROR;
            }

            // get the next available fd
            fd_t vfsfd = alloc_next_fd();
            // Load the appropriate properties into the table
            fdt[vfsfd].fdtp = vfspdt[i].fdtp;
            fdt[vfsfd].pos = (fd_pos_t){ 0, 0 };
            fdt[vfsfd].present = true;

            // Depending on the type of fd,
            // Load the rest of the properties into the table
            switch (fdt[vfsfd].fdtp)
            {
            case FD_FILE:
                fdt[vfsfd].file.fstp = vfspdt[i].fstp;
                fdt[vfsfd].file.partition_mount = i;
                fdt[vfsfd].file.underlying_fd = underlying;
                break;
            
            default:
                break;
            }
            return vfsfd;
        }
    }

    // File not found
    __set_errno( ENOENT );
    return OS32_ERROR;
}
size_t vfs_write( fd_t fd, const char* data, size_t bytes )
{
    VFS_ASSERT_FD(fd)
    if (fdt[fd].fdtp == FD_FILE)
    {
        // return vfs_write_file( fd, data, bytes );
    } 
    else if (fdt[fd].fdtp == FD_STDSTREAM)
    {
        return streambuf_write( &fdt[fd].sb, data, bytes );
    }
    return OS32_ERROR;
}


size_t vfs_read( fd_t fd, char* dest, size_t bytes )
{
    // Check for valid fd
    VFS_ASSERT_FD(fd)
    // depending on fd type, do an action
    if (fdt[fd].fdtp == FD_FILE)
    {
        return vfs_read_file( fd, dest, bytes );
    }
    else if (fdt[fd].fdtp == FD_STDSTREAM)
    {
        memcpy
        ( 
            dest, 
            fdt[fd].sb.m_buf + fdt[fd].pos.ipos, 
            bytes > fdt[fd].sb.m_size ? fdt[fd].sb.m_size : bytes
        );
        fdt[fd].pos.ipos += bytes;
        return bytes;
    }
    return OS32_ERROR;
}
size_t vfs_seekg( fd_t fd, size_t amt, int whence )
{
    // chck for valid fd
    VFS_ASSERT_FD(fd)
    switch (whence)
    {
    case VFS_SEEK_CUR:
        fdt[fd].pos.ipos += amt;
        break;
    case VFS_SEEK_SET:
        fdt[fd].pos.ipos = amt;
        break;
    case VFS_SEEK_END:
        {
            struct fstat st;
            if (vfs_fstat( fd, &st ) == OS32_ERROR) return OS32_ERROR;
            fdt[fd].pos.ipos = st.size;
        }
        break;
    default:
        __set_errno(EINVAL);
        return OS32_ERROR;
    }
    return OS32_SUCCESS;
}
size_t vfs_tellg( fd_t fd )
{
    // check valid descriptor
    VFS_ASSERT_FD(fd)
    return fdt[fd].pos.ipos;
}
size_t vfs_seeko( fd_t fd, size_t amt, int whence )
{
    // check valid descriptor
    VFS_ASSERT_FD(fd)
    switch (whence)
    {
    case VFS_SEEK_CUR:
        fdt[fd].pos.opos += amt;
        break;
    case VFS_SEEK_SET:
        fdt[fd].pos.opos = amt;
        break;
    case VFS_SEEK_END:
        {
            struct fstat st;
            if (vfs_fstat( fd, &st ) == OS32_ERROR) return OS32_ERROR;
            fdt[fd].pos.opos = st.size;
        }
        break;
    default:
        __set_errno(EINVAL);
        return OS32_ERROR;
    }
    return OS32_SUCCESS;
}
size_t vfs_tello( fd_t fd )
{
    // check valid descriptor
    VFS_ASSERT_FD(fd)
    return fdt[fd].pos.opos;
}



err_t vfs_close( fd_t fd )
{
    // check valid descriptor
    VFS_ASSERT_FD(fd)
    if (fdt[fd].fdtp == FD_FILE)
    {
        vfs_close_file(fd);
    }
    fdt[fd].present = false;
    return OS32_SUCCESS;
}

// Copy the data from an ext2fstat struct to a general fstat struct
void vfs_ext2stat_cpy( struct fstat* dest, const struct ext2_fstat* src )
{
    dest->atime = src->atime;
    dest->ctime = src->ctime;
    dest->dev = src->dev;
    dest->gid = src->gid;
    dest->inode = src->inode;
    dest->mode = src->mode;
    dest->mtime = src->mtime;
    dest->nlink = src->nlink;
    dest->size = src->size;
    dest->uid = src->uid;
}

err_t vfs_stat(const char* path, struct fstat* buf)
{

    return OS32_SUCCESS;
}
err_t vfs_fstat(fd_t fd, struct fstat* buf)
{
    // check valid descriptor
    VFS_ASSERT_FD(fd)

    switch ( fdt[fd].fdtp )
    {
    case FD_FILE:
        {    
            struct ext2_fstat ext2fstat;
            fs_methods[ fdt[fd].fdtp ].m_fstat
            (
                vfspdt[ fdt[fd].file.partition_mount ].underlying,
                fdt[fd].file.underlying_fd,
                &ext2fstat
            );
            vfs_ext2stat_cpy( buf, &ext2fstat );
        }
        break;
    
    default:
        __set_errno( EBADF );
        return OS32_ERROR;
        break;
    }
    return OS32_SUCCESS;
}
err_t vfs_lstat(const char* path, struct fstat* buf)
{
    return OS32_SUCCESS;
}

err_t vfs_close_file( fd_t fd )
{
    const fstp_methods_t* m = &fs_methods[ fdt[fd].file.fstp ];
    return m->m_close
    ( 
        vfspdt[ fdt[fd].file.partition_mount ].underlying, 
        fdt[fd].file.underlying_fd 
    );
}

size_t vfs_read_file( fd_t fd, char* dest, size_t bytes )
{
    const fstp_methods_t* m = &fs_methods[ fdt[fd].file.fstp ];
    return m->m_read
    ( 
        vfspdt[ fdt[fd].file.partition_mount ].underlying, 
        fdt[fd].file.underlying_fd, 
        dest,
        bytes,
        fdt[fd].pos.ipos
    );
}

size_t vfs_write_file( fd_t fd, const char* src, size_t bytes )
{
    const fstp_methods_t* m = &fs_methods[ fdt[fd].file.fstp ];
    return m->m_write
    ( 
        vfspdt[ fdt[fd].file.partition_mount ].underlying, 
        fdt[fd].file.underlying_fd, 
        src,
        bytes,
        fdt[fd].pos.opos
    );
}
err_t vfs_setup_proc( struct process* proc )
{
    fd_t in, out, err;
    in = alloc_next_fd();
    out = alloc_next_fd();
    err = alloc_next_fd();

    proc->local_fdt[0] = in;
    proc->local_fdt[1] = out;
    proc->local_fdt[2] = err;

    fdt[in].present = true;
    fdt[in].fdtp = FD_STDSTREAM;
    fdt[in].pos.ipos = 0;
    fdt[in].pos.opos = 0;
    streambuf_create( &fdt[in].sb );

    fdt[out].present = true;
    fdt[out].fdtp = FD_STDSTREAM;
    fdt[out].pos.ipos = 0;
    fdt[out].pos.opos = 0;
    streambuf_create( &fdt[out].sb );

    fdt[err].present = true;
    fdt[err].fdtp = FD_STDSTREAM;
    fdt[err].pos.ipos = 0;
    fdt[err].pos.opos = 0;
    streambuf_create( &fdt[err].sb );
    return OS32_SUCCESS;
}

err_t vfs_clean_proc( struct process* proc )
{
    if (!proc) return OS32_ERROR;
    fd_t in, out, err;
    in = proc->local_fdt[0];
    out = proc->local_fdt[1];
    err = proc->local_fdt[2];

    fdt[in].present = false;
    fdt[out].present = false;
    fdt[err].present = false;

    streambuf_destroy( &fdt[in].sb );
    streambuf_destroy( &fdt[out].sb );
    streambuf_destroy( &fdt[err].sb );

    return OS32_SUCCESS;

}

