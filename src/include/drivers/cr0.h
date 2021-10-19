#ifndef OS32_CR0
#define OS32_CR0
#include <stdint.h>

// https://wiki.osdev.org/CPU_Registers_x86

#define CR0_GET_BIT( cr0, bit )         ((cr0)&~(1<<(bit)))
#define CR0_SET_BIT( cr0, bit, value )  ( (value) ? ((cr0)|=(1<<(bit))) : ((cr0)&=~(1<<(bit))))

#define CR0_PE 0
#define CR0_MP 1
#define CR0_EM 2
#define CR0_TS 3
#define CR0_ET 4
#define CR0_NE 5
#define CR0_WP 16
#define CR0_AM 18
#define CR0_NW 29
#define CR0_CD 30
#define CR0_PG 31

uint32_t cr0_get();
void cr0_set( uint32_t cr0 );



#endif