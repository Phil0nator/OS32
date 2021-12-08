#ifndef OS32_DIRENT
#define OS32_DIRENT
#include "types.h"
#include "bits.h"

struct dirent
{
    ino_t d_ino;
    filetp_t d_tp; 
    char d_name[PATH_MAX];
};



#endif