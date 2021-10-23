#include "system/filesystems/vfs.h"
#include "stdlib/kmalloc.h"
#include "stdlib/string.h"
#include "system/filesystems/linitrd.h"

#define VFS_ASSERT_FD(fd) if (fd < 0){__set_errno(EBADF);return OS32_ERROR;}

typedef int fd_type;
#define FD_FILE 0
#define FD_STDSTREAM 1
#define FD_SOCK 2

typedef int fs_type;
#define FD_EXT2 0

typedef struct fd_pos
{
    size_t ipos;
    size_t opos;
} fd_pos_t;

typedef struct fd_entry
{
    bool present;
    fd_type fdtp;
    union
    {
        struct fd_file
        {
            fs_type fstp;
            fd_t partition_mount;
            fd_t underlying_fd;
        } file;
    };
    fd_pos_t pos;
    
} fd_entry_t;

typedef struct vfs_partition
{
    bool present;
    fs_type fstp;
    void* underlying;
    char mountpoint[VFS_MAX_PATH]; 
} vfs_partition_t;


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

static const fstp_methods_t fs_methods[1] =
{
    {
        ext2_open,
        ext2_read,
        ext2_write,
        ext2_close,
        ext2_stat,
        ext2_fstat,
        ext2_lstat
    }
};

static fd_entry_t fdt[VFS_MAX_OPEN];
static vfs_partition_t vfspdt[VFS_MAX_MEDIA];

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

err_t alloc_mount_partition( fs_type t, void* u, char* m )
{
    for (size_t i = 0; i < VFS_MAX_MEDIA; i++)
    {
        if (!vfspdt[i].present)
        {
            vfspdt[i].present = true;
            vfspdt[i].fstp = t;
            vfspdt[i].underlying = u;
            strcpy( vfspdt[i].mountpoint, m );
            return i;
        }
    }
    return OS32_ERROR;
}


err_t __install_vfs()
{
    alloc_mount_partition( FD_EXT2, __initrd, "/initrd" );
    return OS32_SUCCESS;
}


fd_t vfs_open( const char* fpath, int mode )
{

}
size_t vfs_write( fd_t fd, const char* data, size_t bytes );


size_t vfs_read( fd_t fd, char* dest, size_t bytes )
{
    VFS_ASSERT_FD(fd)

}
size_t vfs_seekg( fd_t fd, size_t amt, int whence )
{
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
    VFS_ASSERT_FD(fd)
    return fdt[fd].pos.ipos;
}
size_t vfs_seeko( fd_t fd, size_t amt, int whence )
{
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
    VFS_ASSERT_FD(fd)
    return fdt[fd].pos.opos;
}



err_t vfs_close( fd_t fd )
{
    VFS_ASSERT_FD(fd)
    if (fdt[fd].fdtp == FD_FILE)
    {
        vfs_close_file(fd);
    }
}

err_t vfs_close_file( fd_t fd )
{
    fstp_methods_t* m = &fs_methods[ fdt[fd].file.fstp ];
    return m->m_close
    ( 
        vfspdt[ fdt[fd].file.partition_mount ].underlying, 
        fdt[fd].file.underlying_fd 
    );
}

size_t vfs_read_file( fd_t fd, char* dest, size_t bytes )
{
    fstp_methods_t* m = &fs_methods[ fdt[fd].file.fstp ];
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
    fstp_methods_t* m = &fs_methods[ fdt[fd].file.fstp ];
    return m->m_write
    ( 
        vfspdt[ fdt[fd].file.partition_mount ].underlying, 
        fdt[fd].file.underlying_fd, 
        src,
        bytes,
        fdt[fd].pos.opos
    );
}