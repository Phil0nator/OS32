#ifndef OS32_INSTRS
#define OS32_INSTRS
#define OS32_MAKEINSTR( s ) __asm__ __volatile__ ( s );
#define __hlt OS32_MAKEINSTR("hlt")
#define __sti OS32_MAKEINSTR("sti")
#define __cli OS32_MAKEINSTR("cli") 
#define __get_esp(dest) __asm__ __volatile__ ( "mov %%esp, %0" : "=r"(dest) ) 
#define __get_ebp(dest) __asm__ __volatile__ ( "mov %%ebp, %0" : "=r"(dest) ) 
#define __set_r(dest, src) __asm__ __volatile__( "mov %%" dest ", %0"  : "=r"(src) )
#define __set_esp(src) __asm__ __volatile__ ( "mov %0, %%esp" : "=r" (src) )
#define __set_ebp(src) __asm__ __volatile__ ( "mov %0, %%ebp" : "=r" (src) )
#define __long_jump(addr, reg) __asm__ __volatile ("mov %%" reg ", %0; jmp *%%" reg : "=r"(addr))


uint32_t __get_eip();



#endif