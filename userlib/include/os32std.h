#ifndef OS32STD
#define OS32STD
#include <stddef.h>
#include <string.h>

#define PATH_MAX 256



int read( int fd, void* buf, size_t count );
int write( int fd, const void* buf, size_t count );

int getcwd(char* buf, size_t len);
int chdir( const char* path );
int fchdir( int fd );
int fork();
int execve( const char* filename, const char* argv, const char* envp );

#endif