#ifndef OS32STD
#define OS32STD
#include <stddef.h>
#include <string.h>
#include "types.h"
#define PATH_MAX 256
#define OS32_SUCCESS (0)
#define OS32_ERROR (-1)
#define OS32_FAILED ((void*)-1)


int read( int fd, void* buf, size_t count );
int write( int fd, const void* buf, size_t count );

int getcwd(char* buf, size_t len);
int chdir( const char* path );
int fchdir( int fd );
int fork();
int execve( const char* filename, const char** argv, const char* envp );
int stat( const char* filename, struct fstat* buf );
int fstat( int fd, struct fstat* buf );
int lstat( const char* filename, struct fstat* buf );

#endif