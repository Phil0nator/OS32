#ifndef OS32_ELF
#define OS32_ELF
#include "stdlib/error.h"
#include "process.h"

typedef void (*elf_fn)();
typedef const char* elfdat_t;

// opaque type
struct elf_file;

/**
 * @brief load ELF metadata into a elf_file struct
 * 
 * the returned pointer must be free'd with elf_free.
 * This can be safely done BEFORE the loaded process
 * is ran because all the data is copied elsewhere.
 * 
 * @param elfdat raw pointer to elf data
 * @return struct elf_file* elf file struct
 */
struct elf_file* elf_load( elfdat_t elfdat );

/**
 * @brief free data used by an elf_file (NOT including raw data)
 * 
 * This can be used safely before starting the loaded process
 * because all the data used by the process is copied elsewhere.
 * 
 * @param e elf struct
 * @return elfdat_t raw pointer (the one originally provided in elf_load)
 */
elfdat_t elf_free( struct elf_file* e );


/**
 * @brief load an elf_file for execution
 * 
 * (WARNING) at the time of calling this function, the current page directory
 * must be the given process's page directory.
 * 
 * @param elf elf metadata struct
 * @param proc process to populate
 * @return elf_fn the entrypoint of the program
 */
elf_fn elf_load_for_exec( struct elf_file* elf, process_t* proc );


#endif