#ifndef OS32_INSTRS
#define OS32_INSTRS
#define OS32_MAKEINSTR( s ) __asm__ __volatile__ ( s );
#define __hlt OS32_MAKEINSTR("hlt")
#define __sti OS32_MAKEINSTR("sti")
#define __cli OS32_MAKEINSTR("cli") 
#define __get_esp(dest) asm volatile ( "mov %%esp, %0" : "=r"(dest) ) 
#define __get_ebp(dest) asm volatile ( "mov %%ebp, %0" : "=r"(dest) ) 
#define __set_esp(src) asm volatile ( "mov %0, %%esp" : : "r" (src) )
#define __set_ebp(src) asm volatile ( "mov %0, %%ebp" : : "r" (src) )
#define __flush_regs(x) asm volatile ("" ::: "%eax", "%ebx", "%ecx", "%edx", "%edi", "%esi")

typedef unsigned long int eflags_t;

#define __get_eflags( dest ) asm volatile ("pushf; pop %%eax;" : "=a"(dest))
#define __set_eflags(  src ) asm volatile ("push %0; popf" : : "r"(src))


uint32_t __get_eip();



#endif