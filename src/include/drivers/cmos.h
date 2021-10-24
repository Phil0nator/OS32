#ifndef OS32_CMOS_D
#define OS32_CMOS_D
#include <stdint.h>
#include <stddef.h>
/**
 * an 8 bit unsigned integer representing a specific
 * CMOS register
 */
typedef uint8_t cmos_reg_t;

/**
 * Write a value to a specific CMOS register
 */
void cmos_write( cmos_reg_t reg, uint8_t value );
/**
 * Read a value from a specific CMOS register
 */
uint8_t cmos_read( cmos_reg_t reg );

// TODO:
void cmos_writes( cmos_reg_t sreg, const uint8_t* values, size_t size );
void cmos_reads( cmos_reg_t sreg, uint8_t* dest, size_t size );

#endif