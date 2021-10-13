#ifndef OS32_IOINSTRS
#define OS32_IONSTRS
#include <stdint.h>
extern void outportb( int port, uint8_t byte );
extern uint8_t inportb( int port );


#endif