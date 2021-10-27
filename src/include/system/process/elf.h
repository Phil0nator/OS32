#ifndef OS32_ELF
#define OS32_ELF
#include "stdlib/error.h"
#include "process.h"

typedef void (*elf_fn)();
typedef const char* elfdat_t;

// opaque type
struct elf_file;

struct elf_file* elf_load( elfdat_t elfdat );
elfdat_t elf_free( struct elf_file* e );

elf_fn elf_load_for_exec( struct elf_file* elf, process_t* proc );
elf_fn elf_get_sym( struct elf_file* elf, const char* symbol );
err_t elf_unload_from_exec( struct elf_file* elf, process_t* proc );

#endif