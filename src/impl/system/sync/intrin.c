#include "system/sync/intrin.h"

int cmpxchg( volatile int* x, int val, int newval )
{ 
    unsigned char ret; 
    __cmpxchg(x,val,newval,ret); 
    return ret; 
}
