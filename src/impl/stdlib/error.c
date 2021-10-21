#include "stdlib/error.h"
#include "stdlib/string.h"

static errno_t __errno = 0;

errno_t __get_errno()
{
    return __errno;
}

void __set_errno( errno_t e )
{
    __errno = e;
}

void kpanic( const char* preface )
{
    
}