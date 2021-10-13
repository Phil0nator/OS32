#ifndef OS32_INSTRS
#define OS32_INSTRS
#define OS32_MAKEINSTR( s ) __asm__ __volatile__ ( s );
#define __hlt OS32_MAKEINSTR("hlt")
#define __sti OS32_MAKEINSTR("sti")
#define __cli OS32_MAKEINSTR("cli") 
#endif