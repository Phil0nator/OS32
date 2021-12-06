#ifndef OS32_TYPES
#define OS32_TYPES
#include <stdint.h>
typedef int errno_t;

// File descriptor
typedef int fd_t;
// Device id
typedef int dev_t;
// User id
typedef int uid_t;
// Group id
typedef int gid_t;
// UNIX time
typedef unsigned long pxtime_t;

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