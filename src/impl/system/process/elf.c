#include "system/process/elf.h"
#include "stdlib/string.h"
#include "stdlib/kmalloc.h"
#include "stdlib/error.h"
#include <elf.h>

#define ELF_SIGNATURE (ELFMAG0 + (ELFMAG1 << 8) + (ELFMAG2 << 16) + (ELFMAG3 << 24) )
#define ELF_THROW_EPERM __set_errno( EPERM ); return OS32_ERROR;
#define ELF_SIG_ASSERT(e_ident) if( *(int*)(e_ident) != ELF_SIGNATURE ) { ELF_THROW_EPERM }
#define ELF_EI_ASSERT( e_ident, idx, val ) if ( e_ident[idx] != val ) { ELF_THROW_EPERM }

typedef struct elf_file
{
    Elf32_Ehdr* header;
    Elf32_Shdr* sections;
    
} elf_file_t;

err_t elf_check_validity( Elf32_Ehdr* h )
{
    ELF_SIG_ASSERT( h->e_ident )
    ELF_EI_ASSERT( h->e_ident, EI_CLASS, ELFCLASS32 )
    ELF_EI_ASSERT( h->e_ident, EI_VERSION, EV_CURRENT )
    ELF_EI_ASSERT( h->e_ident, EI_DATA, ELFDATA2LSB )
    if ( h->e_machine != EM_386 )
    {
        ELF_THROW_EPERM
    }
    if ( h->e_type != ET_REL && h->e_type != ET_EXEC )
    {
        ELF_THROW_EPERM
    }
    return OS32_SUCCESS;
}

struct elf_file* elf_load( elfdat_t elfdat )
{
    struct elf_file* out = kmalloc(sizeof(struct elf_file));
    out->header = (Elf32_Ehdr*) elfdat;
    if (elf_check_validity( out->header ) == OS32_ERROR)
    {
        return OS32_FAILED;
    }
    out->sections = (Elf32_Shdr*)(((char*)out->header) + out->header->e_shoff);

}
elfdat_t elf_free( struct elf_file* e )
{
    elfdat_t out = (elfdat_t)e->header;
    kfree(e);
    return out;
}



elf_fn elf_load_for_exec( struct elf_file* elf )
{



    return elf->header->e_entry;
}
elf_fn elf_get_sym( struct elf_file* elf, const char* symbol )
{

}
err_t elf_unload_from_exec( struct elf_file* elf )
{

}
