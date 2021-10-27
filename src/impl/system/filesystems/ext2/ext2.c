#include "system/filesystems/ext2/ext2.h"
#include "stdlib/time.h"
#include "stdlib/kmalloc.h"
#include "stdlib/string.h"
#include <stddef.h>
#include <stdint.h>

typedef uint16_t ext2_fsstate;
#define EXT2_STATE_CLEAN 1
#define EXT2_STATE_ERROR 2

typedef uint16_t ext2_erract;
#define EXT2_ERRACT_IGNRE 1
#define EXT2_ERRACT_REMNT 2
#define EXT2_ERRACT_PANIC 3

typedef uint32_t ext2_osid;
#define EXT2_OSID_LINUX     0
#define EXT2_OSID_HURD      1
#define EXT2_OSID_MASIX     2
#define EXT2_OSID_BSD       3
#define EXT2_OSID_LITES     4



typedef uint32_t ext2_optionals;
#define EXT2_OPTIONAL_PREALLOCDIR   0x0001
#define EXT2_OPTIONAL_AFS_INODES    0x0002
#define EXT2_OPTIONAL_FSJ           0x0004
#define EXT2_OPTIONAL_INODE_EXT     0x0008
#define EXT2_OPTIONAL_RESIZABLE     0x0010
#define EXT2_OPTIONAL_DIRHASH       0x0020

typedef uint32_t ext2_reqopts;
#define EXT2_REQOPT_COMPRESS 0x0001
#define EXT2_REQOPT_DIRENT_T 0x0002
#define EXT2_REQOPT_REP_JRNL 0x0004
#define EXT2_REQOPT_JRNL_DEV 0x0008

typedef uint32_t ext2_rofeats;
#define EXT2_ROFEAT_SPARSE  0x0001
#define EXT2_64             0x0002
#define EXT2_BINTREE        0x0004

typedef uint16_t ext2_tp_perms_t;
#define EXT2_TP_FIFO        0x1000
#define EXT2_TP_CHDEV       0x2000
#define EXT2_TP_DIR         0x4000
#define EXT2_TP_BLDEV       0x6000
#define EXT2_TP_REG         0x8000
#define EXT2_TP_SYMLN       0xA000
#define EXT2_TP_UNIXSOCK    0xC000

#define EXT2_PERM_OTH_EXEC  0x001
#define EXT2_PERM_OTH_WRITE 0x002
#define EXT2_PERM_OTH_READ  0x004
#define EXT2_PERM_GRP_EXEC  0x008
#define EXT2_PERM_GRP_WRITE 0x010
#define EXT2_PERM_GRP_READ  0x020
#define EXT2_PERM_USR_EXEC  0x040
#define EXT2_PERM_USR_WRITE 0x080
#define EXT2_PERM_USR_READ  0x100
#define EXT2_PERM_STICKY    0x200
#define EXT2_PERM_SET_GRPID 0x400
#define EXT2_PERM_SET_USRID 0x800

typedef uint32_t ext2_inode_flags_t;
#define EXT2_FLAG_SECDEL    0x00000001
#define EXT2_FLAG_CPYDEL    0x00000002
#define EXT2_FLAG_COMPRESS  0x00000004
#define EXT2_FLAG_SYNC      0x00000008
#define EXT2_FLAG_IMMUTABLE 0x00000010
#define EXT2_FLAG_APPEND    0x00000020
#define EXT2_FLAG_NODUMP    0x00000040
#define EXT2_FLAG_NO_LAT    0x00000080
#define EXT2_FLAG_HASHDIR   0x00010000
#define EXT2_FLAG_AFSDIR    0x00020000
#define EXT2_FLAG_JRNLDAT   0x00040000


#define EXT2_ROOT_INODE     2

typedef uint8_t ext2_dirent_type_t;
#define EXT2_DIRENT_TYPE_UKNWN      0
#define EXT2_DIRENT_TYPE_REG        1
#define EXT2_DIRENT_TYPE_DIR        2
#define EXT2_DIRENT_TYPE_CHARDEV    3
#define EXT2_DIRENT_TYPE_BLKDEV     4
#define EXT2_DIRENT_TYPE_FIFO       5
#define EXT2_DIRENT_TYPE_SOCK       6
#define EXT2_DIRENT_TYPE_SYMLN      7


#define EXT2_EXTENDEDSUPER_VERNO    1
#define EXT2_ROOT_INODE             2


#pragma pack(1)
//https://wiki.osdev.org/Ext2
typedef struct ext2_base_superblock
{
    uint32_t inodes;        // inode count
    uint32_t blocks;        // block count
    uint32_t su_blocks;     // dedicated superuser blocks
    uint32_t fr_blocks;     // free blocks
    uint32_t fr_inodes;     // free inodes
    uint32_t sb_bno;        // blocknumber of the superblock
    uint32_t bl_size;       // log2(blocksize)
    uint32_t fr_size;       // log2(fragment size)
    uint32_t bl_grp;        // blocks per group
    uint32_t fr_grp;        // fragments per group
    uint32_t in_grp;        // inodes per group
    pxtime_t lmt;           // last mount time (POSIX time)
    pxtime_t lwt;           // last write time
    uint16_t mcount;        // times the volume has been mounted since its last consistency check
    uint16_t maxmcount;     // max number of times the system can be mounted before a consistency check
    uint16_t sig;           // signature (0xef53)
    ext2_fsstate state;     // file system state
    ext2_erract erract;     // action on error
    uint16_t ver_min;       // minor version
    pxtime_t lcc;           // last consistency check
    pxtime_t fcci;          // interval between forced consistency checks
    ext2_osid osid;         // OS id
    uint32_t ver_maj;       // major version
    ext2_uid uid_res;       // user ID that can use reserved blocks
    ext2_gid gid_res;       // group id that can use reserved blocks 
} ext2_base_superblock_t;

typedef struct ext2_superblock_extended
{
    ext2_base_superblock_t base;
    uint32_t fnr_inode;     // first non-reserved inode
    uint16_t inode_s;       // size of inode in bytes
    uint16_t this_blg;      // the block group containing the superblock
    ext2_optionals feat;    // optional features present
    ext2_reqopts reqfeat;   // required extra features
    ext2_rofeats rofeats;   // features which if not present require the fs to be loaded in read only mode
    char fsid[16];          // filesystem id
    char volname[16];       // volume name
    char lmount[64];        // path the volume was last mounted to
    uint32_t cmprss_alg;    // compression algorithm used
    uint8_t  prealloc_f;    // blocks to preallocate for files
    uint8_t  prealloc_d;    // blocks to preallocate for directories
    uint16_t _;             // unused
    char jid[16];           // journal id
    uint32_t jrnl_inode;    // journal inode
    uint32_t jrnl_device;   // journal device
    uint32_t orph_inode_h;  // head of orphan inode list
    char pad[788];          // padding
} ext2_superblock_extended_t;


typedef struct ext2_block_group_descriptor
{
    uint32_t blk_usg;       // block address of block usage bitmap
    uint32_t inode_usg;     // block address of inode usage bitmap
    uint32_t inodes_start;  // block address of start of inode table
    uint16_t blk_free;      // number of unallocated blocks in group
    uint16_t inode_free;    // number of unallocated inodes in group
    uint16_t grp_dirs;      // number of directories in group
    char pad[14];
} ext2_block_group_descriptor_t;

typedef struct ext2_inode
{
    ext2_tp_perms_t tp_perm;    // inode type and permissions
    ext2_uid uid;               // user ID
    uint32_t size_low;          // lower 32 bits of size
    pxtime_t la_time;           // last access time
    pxtime_t create_time;       // creation time
    pxtime_t lm_time;           // last modification time
    pxtime_t del_time;          // deletion time
    ext2_gid gid;               // group id
    uint16_t hardln_count;      // number of hard links to this inode
    uint32_t sec_use;           // number of disk sectors in use by this inode (not including inode struct or dir entries)
    ext2_inode_flags_t flags;   // flags
    uint32_t osspec1;           // os-specific value
    uint32_t direct[12];        // direct pointers
    uint32_t single_indr;       // pointer to singly indirect block
    uint32_t double_indr;       // pointer to doubly indirect block
    uint32_t triple_indr;       // pointer to triply indirect block
    uint32_t genno;             // generation number
    union                       // either reserved in version 0, or extended attribute block (file acl)
    {
        uint32_t reserved_v0;
        uint32_t facl;
    };
    union
    {
        uint32_t reserved_v0_;  // reserved in version 0
        uint32_t size_high;     // upper 32 bits of a file size (when feature bit is set, and inode is a file)
        uint32_t diracl;        // directory ACL if !version0, and inode is directory
    };
    uint32_t frag_addr;         // block address of fragment
    
    union osspec2_u             // second os-specific value
    {
        struct osspec2_linux
        {
            uint8_t fragno;     // fragment number
            uint8_t fragsize;   // fragment size
            uint16_t __reserved;
            uint16_t uid_high;  // high 16 bits of uid
            uint16_t gid_high;  // high 16 bits of gid
            uint32_t _reserved; 
        } os_linux;
        struct osspec2_hurd
        {
            uint8_t fragno;     // fragment number
            uint8_t fragsize;   // fragment size
            uint16_t tp_perm_high; // type & permissions field high 16 bits
            uint16_t uid_high;  // high 16 bits of uid
            uint16_t gid_high;  // high 16 bits of gid
            uint32_t author_uid32; // 32 bit author uid
        } os_hurd;
        struct osspec2_masix
        {
            uint8_t fragno;     // fragment number
            uint8_t fragsize;   // fragment size
        } os_masix;
    } osspec2;
} ext2_inode_t;

typedef struct ext2_dirent_head
{
    uint32_t inode;             // inode
    uint16_t size;              // size of this entry (including all fields)
    union                       // type specifier if 
                                // "directory entries have file type byte" 
                                // feature is set. Otherwise, higher 8 bits of namelen
    {
        uint16_t namelen16;     // 16 bit name length
        struct {
            uint8_t namelen8;   // 8 bit name length
            ext2_dirent_type_t type;    // type specifiers
        };
    };    
} ext2_dirent_head_t;

typedef struct fd_rel
{
    ext2_inode_t* inode;
} fd_rel_t;

typedef struct ext2_partition
{
    char* raw_data;
    union
    {
        ext2_base_superblock_t* base_superbock;
        ext2_superblock_extended_t* extended_superblock;
    };

    ext2_block_group_descriptor_t* bgdt;
    ext2_inode_t* root;

    size_t block_size;
    size_t frag_size;
    size_t inode_size;


    fd_rel_t relations[EXT2_MAX_OPEN];
    
} ext2_partition_t;

ext2_inode_t* ext2_get_inode( struct ext2_partition* src, size_t index )
{
    // Find which group the indode belongs to
    size_t group = (index-1) / src->base_superbock->in_grp;
    // find the block in which the inode resides
    // size_t grouped_idx = (index-1) % src->base_superbock->in_grp;
    // find the block index in which the inode resides
    // size_t block = ( grouped_idx * src->inode_size ) / src->block_size;
    // find the inode table in which the inode resisdes
    // char* table = src->raw_data + src->bgdt[ group ].inodes_start * src->block_size;
    // inode address = (data start) + ( inode table ) + ( index )
    return (ext2_inode_t*)( src->raw_data + ( src->bgdt[group].inodes_start ) * src->block_size + ( (index-1) * src->inode_size ));
}

char* ext2_get_block( struct ext2_partition* src, uint32_t blockno )
{
    // block = (data start) + (block index * block size)
    return (src->raw_data)+blockno*src->block_size;
}

/**
 * Read from the direct blocks of an inode
 */
size_t ext2_read_direct( struct ext2_partition* p, ext2_inode_t* src, char* buf, size_t bytes, size_t start )
{
    // bytes read
    size_t total_read = 0;
    // direct block to start read
    size_t direct_idx = start / p->block_size;
    // direct block to end read
    size_t last_block = (start+bytes) / p->block_size;
    // for each block to read
    for(; direct_idx <= last_block; direct_idx++)
    {
        // get block number
        uint32_t blockno = src->direct[direct_idx];
        // get block address
        char* data = ext2_get_block(p, blockno);
        // determine the amount to read from the block
        size_t readamt = bytes < p->block_size ? bytes : p->block_size;
        // read the data into the buffer
        memcpy( buf, data, readamt );
        // update counters and advance the buffer
        buf += readamt;
        total_read += readamt;
        bytes -= readamt;
    }
    return total_read;
}

// Read bytes from an inode
size_t ext2_read_bytes( struct ext2_partition* p, ext2_inode_t* src, char* buf, size_t bytes, size_t start )
{
    // bytes-read counter
    size_t total_read = 0;
    // if the program should start reading in the direct
    // blocks of the inode...
    if (start/p->block_size <= 12)
    {
        // use the specialized ext2_read_direct function
        size_t bytes_read = ext2_read_direct( p, src, buf, bytes, start );
        // update counters and advance buffer
        bytes -= bytes_read;
        start += bytes_read;
        buf   += bytes_read;
        total_read += bytes_read;
    }

    return total_read;
}

uint32_t ext2_find_in_dir(ext2_partition_t* p, ext2_inode_t* dir, const char* name )
{
    // buffer to store one chunk of the table at a time
    char buffer[1024];
    // while true...
    for(size_t iter = 0; 1; iter++ )
    {
        // read data from the table into the buffer
        if (ext2_read_bytes( p, dir, buffer, sizeof(buffer), iter ) <= 0) return OS32_ERROR;
        for 
        (
            // find the direntry
            ext2_dirent_head_t* head = (ext2_dirent_head_t*) buffer; 
            // while there are more direntries in this buffer
            (char*)head != buffer+sizeof(buffer); 
            // update dirent to the next dirent
            head = (ext2_dirent_head_t*)(((char*)head)+head->size)
        )
        {
            // get the name of the dirent
            char* cur_name = ((char*)head)+sizeof(ext2_dirent_head_t);
            // if the name matches the requested name,
            if (strequ(cur_name, name))
            {
                // return the inode number
                return head->inode;
            }
        } 
    }
    // File does not exist
    __set_errno( ENOENT );
    return OS32_ERROR;
}

/**
 * Get the inode of a file using the path to that file
 */
ext2_inode_t* ext2_getf( ext2_partition_t* src, const char* path, bool syml )
{
    // All paths must be absolute
    if (path[0] != '/') return OS32_FAILED;
    // The root path is an easy return value
    if ( strequ( path, "/" ) ) return src->root;
    // cut off the '/' from the beginning of path
    path++;

    // fname is a temporary buffer for path during the loop
    char fname[256];
    strcpy(fname, path);

    // starting directory is root
    ext2_inode_t* cur_dir = src->root;
    
    while (true)
    {
        // find the next slash in the path
        uint32_t next_slash = strchr(path, '/');
        // If there is another slash in the path,
        if (next_slash != -1ul)
        {
            // copy this piece of path and advance
            // ie. /home/beans/Documents
            //           ^    ^
            //      fname will equal "beans"
            //      path will equal "Documents"
            // 
            bzero(fname, sizeof(fname));
            memcpy( fname, path, next_slash );
            path+=next_slash+1;
        }
        else
        {
            // If this is the end of the path,
            // ie. /home/beans/Documents
            //                ^         >
            //          fname will equal the last element: "Documents"
            //          path is no longer needed
            bzero(fname, sizeof(fname));
            strcpy(fname, path);
        }
        // Find the inode number for this component of the path
        uint32_t next_inode = 0;
        if ( (next_inode = ext2_find_in_dir( src, cur_dir, fname )) == (size_t)OS32_ERROR )
        {
            // If the entry could not be found, the file/dir doesn't exist
            return OS32_FAILED;
        }
        // update the current directory with the inode number
        cur_dir = ext2_get_inode( src, next_inode );
        // If at the end of the path, break the loop
        if (next_slash == -1ul) break;
    }
    // For symbolic links:
    // TODO: fix so it doesn't always run

    if ( false && syml && (cur_dir->tp_perm & EXT2_TP_SYMLN))
    {
        // If symbolic links are enabled for this search,
        // read the address held by the link and recursively
        // search for that file/dir
        bzero(fname, sizeof(fname));
        ext2_read_bytes( src, cur_dir, fname, cur_dir->size_low, 0 );
        return ext2_getf( src, fname, syml );
    }
    // return the located file/dir
    return cur_dir;
}

err_t ext2_init( struct ext2_partition** _dest, char* raw )
{
    // Allocate an ext2 partition for the user
    *_dest = kmalloc( sizeof(struct ext2_partition) );
    // ease of access
    struct ext2_partition* dest = *_dest;
    // zero out the partition
    bzero((char*)*_dest, sizeof(struct ext2_partition));
    // load in the raw pointer
    dest->raw_data = raw;
    // identify superblock
    dest->base_superbock = (ext2_base_superblock_t*)( raw + 1024);
    // ensure that this is a valid ext2 partition
    if (dest->base_superbock->sig != EXT2_SIGNATURE)
    {
        __set_errno( EPERM );
        return OS32_ERROR;
    }
    // determine block and fragment size
    dest->block_size = 1024 << dest->base_superbock->bl_size;
    dest->frag_size = 1024 << dest->base_superbock->bl_size;
    // locate the block group descriptor table
    dest->bgdt = (ext2_block_group_descriptor_t*) (raw + dest->block_size);
    // depending on the version of this ext2 partition,
    // different values will be used.
    if (dest->base_superbock->ver_maj >= 1)
    {
        // for all versions >= 1, the extended superblock may be used
        dest->inode_size = dest->extended_superblock->inode_s;
    }
    else
    {
        // for versions < 1, many values are hardcoded
        dest->inode_size = 128;
    }
    // The root inode is always inode 2:
    dest->root = ext2_get_inode( dest, EXT2_ROOT_INODE );
    return OS32_SUCCESS;
}
void ext2_free( struct ext2_partition* p, bool freeRaw )
{
    kfree(p);
}

fd_t ext2_open( struct ext2_partition* p, const char* fname )
{
    // check to make sure this is valid partition
    if (p->base_superbock->sig != EXT2_SIGNATURE)
    {
        __set_errno(EPERM);
        return OS32_ERROR;
    }
    // Find the file inode
    ext2_inode_t* in = ext2_getf( p, fname, true );
    if (in == OS32_FAILED) return OS32_ERROR;
    
    // to find a file descriptor, 
    // start at #1, and search through the existing 
    // descriptor table to find the next empty entry.
    fd_t fd = 1;
    for (; fd < EXT2_MAX_OPEN; fd++)
    {
        // for an empty entry the inode field will be NULL
        if ( !p->relations[fd].inode )
        {
            p->relations[fd].inode = in;
            return fd;
        }
    }

    // Too many open files, no empty entry could be found
    __set_errno(EMFILE);
    return OS32_ERROR;
    
}
size_t ext2_write(struct ext2_partition* p, fd_t fd, const char* data, size_t bytes, size_t start )
{

}
size_t ext2_read(struct ext2_partition* p, fd_t fd, char* dest, size_t bytes, size_t start )
{
    // check for valid fd
    if (fd <= 0 || p->relations[fd].inode == 0)
    {
        __set_errno( EBADF );
        return OS32_ERROR;
    }
    // get inode, and read the data
    ext2_inode_t* inode = p->relations[fd].inode;
    return ext2_read_bytes( p, inode, dest, bytes, start );
}
err_t ext2_close(struct ext2_partition* p, fd_t fd )
{
    // check for valid fd
    if (fd <= 0 || p->relations[fd].inode == 0)
    {
        __set_errno( EBADF );
        return OS32_ERROR;
    }
    // reset the entry in the file descriptor table to NULL
    p->relations[fd].inode = 0;
    return OS32_SUCCESS;
}

// translate the inode data over to the fstat structure
void ext2_stat_inodecpy( ext2_inode_t* inode, struct ext2_fstat* buf )
{
    buf->atime = inode->la_time;
    buf->ctime = inode->create_time;
    buf->gid = inode->gid;
    buf->mtime = inode->lm_time;
    buf->uid = inode->uid;
    buf->size = inode->size_low;
    buf->nlink = inode->hardln_count;
}

err_t ext2_stat(struct ext2_partition* p, const char* path, struct ext2_fstat* buf)
{
    fd_t fd;
    // open the file temporarily...
    if ( (fd = ext2_open( p, path )) == OS32_ERROR)
    {
        return OS32_ERROR;
    }
    // get stat data
    err_t out = ext2_fstat( p, fd, buf );
    // close the file again
    ext2_close(p, fd);
    return out;
}
err_t ext2_fstat(struct ext2_partition* p, fd_t fd, struct ext2_fstat* buf)
{
    // check for valid fd
    if (fd <= 0 || p->relations[fd].inode == 0)
    {
        __set_errno(EBADF);
        return OS32_ERROR;
    }
    // get the corresponding inode
    ext2_inode_t* inode = p->relations[fd].inode;
    // copy the inode data to the fstat struct
    ext2_stat_inodecpy( inode, buf );
    return OS32_SUCCESS; 
}
err_t ext2_lstat(struct ext2_partition* p, const char* path, struct ext2_fstat* buf)
{
    // check for valid ext2 partition
    if (p->base_superbock->sig != EXT2_SIGNATURE)
    {
        __set_errno(EPERM);
        return OS32_ERROR;
    }
    // get the file, without symbolics
    ext2_inode_t* in = ext2_getf( p, path, false );
    if (in == OS32_FAILED) return OS32_ERROR;
    // copy the inode data to the fstat struct
    ext2_stat_inodecpy( in, buf );
    return OS32_SUCCESS;
}

#pragma pack(0)
