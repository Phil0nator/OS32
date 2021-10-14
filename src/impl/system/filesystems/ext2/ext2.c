#include "system/filesystems/ext2/ext2.h"
#include "stdlib/time.h"
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

typedef uint16_t ext2_uid;
typedef uint16_t ext2_gid;

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

typedef struct ext2_partition
{
    union
    {
        ext2_base_superblock_t* base_superbock;
        ext2_superblock_extended_t* extended_superblock;
    };
} ext2_partition_t;

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




#pragma pack(0)
