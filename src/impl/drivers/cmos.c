#include "drivers/cmos.h"
#include "stdlib/instructions.h"
#include "stdlib/ioinstrs.h"

#define CMOS_REGSET_PORT 0x70
#define CMOS_DATA_PORT 0x71

void cmos_write( cmos_reg_t reg, uint8_t value )
{
    __cli
    outportb( CMOS_REGSET_PORT, reg );
    outportb( CMOS_DATA_PORT, value );
    __sti
}
uint8_t cmos_read( cmos_reg_t reg )
{
    uint8_t value;
    __cli
    outportb( CMOS_REGSET_PORT, reg );
    value = inportb(CMOS_DATA_PORT);
    __sti
    return value;
}

void cmos_writes( cmos_reg_t sreg, const uint8_t* values, size_t size );
void cmos_reads( cmos_reg_t sreg, uint8_t* dest, size_t size );
