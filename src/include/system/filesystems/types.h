#ifndef OS32_FS_TYPES
#define OS32_FS_TYPES
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "stdlib/time.h"

typedef int fd_t;
typedef int dev_t;
typedef int uid_t;
typedef int gid_t;

struct fstat
{
    dev_t dev;
    uint32_t inode;
    uint32_t mode;
    uint32_t nlink;
    uid_t uid;
    gid_t gid;
    size_t size;
    pxtime_t atime;
    pxtime_t mtime;
    pxtime_t ctime;
};


#endif