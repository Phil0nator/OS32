#include "system/process/elf.h"
#include "stdlib/string.h"
#include "stdlib/error.h"
#include <elf.h>

#define ELF_SIGNATURE (ELFMAG0 + (ELFMAG1 << 8) + (ELFMAG2 << 16) + (ELFMAG3 << 24) )
#define ELF_THROW_EPERM __set_errno( EPERM ); return OS32_ERROR;
#define ELF_SIG_ASSERT(e_ident) if( *(int*)(e_ident) != ELF_SIGNATURE ) { ELF_THROW_EPERM }
#define ELF_EI_ASSERT( e_ident, idx, val ) if ( e_ident[idx] != val ) { ELF_THROW_EPERM }


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
    return OS32_SUCCESS;
}


elf_fn elf_load_for_exec( elfdat_t elf )
{
    Elf32_Ehdr* header_check = elf;
    if (elf_check_validity( header_check ) != OS32_SUCCESS)
    {
        return OS32_FAILED;
    }
}
elf_fn elf_get_sym( elfdat_t elf, const char* symbol )
{
    Elf32_Ehdr* header_check = elf;
    if (elf_check_validity( header_check ) != OS32_SUCCESS)
    {
        return OS32_FAILED;
    }
}
err_t elf_unload_from_exec( elfdat_t elf )
{

}
