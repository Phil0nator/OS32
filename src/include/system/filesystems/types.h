#ifndef OS32_FS_TYPES
#define OS32_FS_TYPES
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "stdlib/time.h"

// File descriptor
typedef int fd_t;
// Device id
typedef int dev_t;
// User id
typedef int uid_t;
// Group id
typedef int gid_t;
// File mode
typedef int mode_t;

struct fstat
{
    dev_t dev;
    uint32_t inode;
    uint32_t mode;
    // Number of hard links
    uint32_t nlink;
    uid_t uid;
    gid_t gid;
    size_t size;
    // Last access time
    pxtime_t atime;
    // Last modification time
    pxtime_t mtime;
    // Creation time
    pxtime_t ctime;
};


#endif