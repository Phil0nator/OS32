#ifndef OS32_SYSCALL_IMPL
#define OS32_SYSCALL_IMPL
#include <stddef.h>

int __s_read( int fd, void* buf, size_t count );
int __s_write( int fd, void* buf, size_t count );

#endif