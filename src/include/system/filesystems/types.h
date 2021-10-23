#ifndef OS32_FS_TYPES
#define OS32_FS_TYPES
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "stdlib/time.h"

typedef int fd_t;
typedef int dev_t;

struct fstat
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


#endif