#ifndef OS32_SYS_STAT
#define OS32_SYS_STAT
#include "types.h"


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

int stat( const char* filename, struct fstat* buf );
int fstat( int fd, struct fstat* buf );
int lstat( const char* filename, struct fstat* buf );


#endif