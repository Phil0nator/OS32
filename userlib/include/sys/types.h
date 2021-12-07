#ifndef OS32_TYPES
#define OS32_TYPES
#include <stdint.h>
#include <stddef.h>
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
// Process ID
typedef int pid_t;


#endif