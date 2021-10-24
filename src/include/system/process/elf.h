#ifndef OS32_ELF
#define OS32_ELF
#include "stdlib/error.h"

typedef void (*elf_fn)();
typedef const char* elfdat_t;


elf_fn elf_load_for_exec( elfdat_t elf );
elf_fn elf_get_sym( elfdat_t elf, const char* symbol );
err_t elf_unload_from_exec( elfdat_t elf );

#endif