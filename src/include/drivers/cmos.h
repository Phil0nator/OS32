#ifndef OS32_CMOS_D
#define OS32_CMOS_D
#include <stdint.h>
#include <stddef.h>
typedef uint8_t cmos_reg_t;

void cmos_write( cmos_reg_t reg, uint8_t value );
uint8_t cmos_read( cmos_reg_t reg );

void cmos_writes( cmos_reg_t sreg, const uint8_t* values, size_t size );
void cmos_reads( cmos_reg_t sreg, uint8_t* dest, size_t size );

#endif