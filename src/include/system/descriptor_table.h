#ifndef OS32_DT
#define OS32_DT
#include <stdlib/streambuf.h>
typedef int descriptor_type;
typedef int fd_t;

#define DT_FSTREAM
#define DT_SOCK

#pragma pack(1)
typedef struct descriptor_meta
{
    char buf[32]; // maximum allowed metadata
} descriptor_meta_t;

typedef struct descriptor_entry
{
    descriptor_type type;
    descriptor_meta_t meta;
    streambuf_t buf;
} descriptor_entry_t;

typedef struct fstream_meta
{
    const char* fname;
    int inode;
} fstream_meta_t;

#pragma pack(0)


void __install_dt();

descriptor_entry_t* dt_get( fd_t fd );


#endif