#ifndef OS32_IOINSTRS
#define OS32_IONSTRS
#include <stdint.h>

/**
 * out x86 instruction
 */
extern void outportb( int port, uint8_t byte );
/**
 * in x86 instruction
 */
extern uint8_t inportb( int port );


#endif