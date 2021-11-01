#include "system/process/elf.h"
#include "stdlib/string.h"
#include "stdlib/kmalloc.h"
#include "stdlib/error.h"
#include <elf.h>

// https://wiki.osdev.org/ELF_Tutorial

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

const char* elf_get_str( elf_file_t* e, size_t offset )
{
    return (((char*)e->header) + e->sections[ e->header->e_shstrndx ].sh_offset) + offset;
}

Elf32_Shdr* elf_get_shdr( elf_file_t* e, size_t idx )
{
    return &e->sections[ idx ];
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
    return out;
}
elfdat_t elf_free( struct elf_file* e )
{
    elfdat_t out = (elfdat_t)e->header;
    kfree(e);
    return out;
}

void elf_load_nobits( struct elf_file* elf, process_t* proc, Elf32_Shdr* sh )
{
    page_table_ent_t perms = {0};
    perms.present = 1;
    perms.rw = (bool)(sh->sh_flags & SHF_WRITE);
    perms.user = 1;
    void* page_aligned_shaddr = (void*)(sh->sh_addr & (~0x03ff));
    
    size_t pgs = ((sh->sh_size + (sh->sh_addr - (uint32_t)page_aligned_shaddr) )  / PAGE_SIZE)+1;

    kmalloc_alloc_pages( 
        proc->pdir, 
        pgs, 
        page_aligned_shaddr, 
        perms
    );
    for (size_t i = 0; i < pgs; i++)
    {
        void* addr = (void*)((uint32_t)page_aligned_shaddr + i * PAGE_SIZE);
        wire_page( 
            &boot_page_directory, 
            phys_addr_of( proc->pdir, addr ),
            addr,
            (page_table_ent_t){.present=1,.rw=1} 
        );
        memset( addr, 0, PAGE_SIZE);
        unwire_page( 
            &boot_page_directory, 
            addr
        );
    }
}

void elf_load_phase_1( struct elf_file* elf, process_t* proc )
{
    for (size_t i = 0; i < elf->header->e_shnum; i++)
    {
        Elf32_Shdr* sh = elf_get_shdr( elf, i );
        if (sh->sh_type == SHT_NOBITS)
        {
            if (!sh->sh_size) continue;
            // If the section exists in memory
            if (sh->sh_flags & SHF_ALLOC)
            {
                elf_load_nobits(elf,proc,sh);
            }
        }
    }
}

int elf_get_symval( struct elf_file* elf, int table, unsigned index )
{
    if (table == SHN_UNDEF || index == SHN_UNDEF) return 0;
    Elf32_Shdr* symtab = elf_get_shdr( elf, table );
    uint32_t entries = symtab->sh_size / symtab->sh_entsize;
    if (index >= entries)
    {
        // oor
        return -1;
    }
    int symaddr = (int)elf->header + symtab->sh_offset;
    Elf32_Sym* symbol = &((Elf32_Sym*)symaddr)[index];
    if (symbol->st_shndx == SHN_UNDEF)
    {
        // Not implimented
        Elf32_Shdr *strtab = elf_get_shdr(elf, symtab->sh_link);
        const char* name = (const char*)elf->header + strtab->sh_offset + symbol->st_name;

        ///https://wiki.osdev.org/ELF_Tutorial


    }
    else if (symbol->st_shndx == SHN_ABS)
    {
        return symbol->st_value;
    }
    else
    {
        Elf32_Shdr *target = elf_get_shdr(elf, symbol->st_shndx);
        return (int)elf->header + symbol->st_value + target->sh_offset;
    }
}

err_t elf_reloc( struct elf_file* elf, Elf32_Rel* reltab, Elf32_Shdr* sh )
{
    Elf32_Shdr * target = elf_get_shdr( elf, sh->sh_info );
    int addr = (int)elf->header + target->sh_offset;
    int *ref = (int*)(addr + reltab->r_offset);

    int symval = 0;
    if (ELF32_R_SYM(reltab->r_info) != SHN_UNDEF)
    {
        symval = elf_get_symval(elf, sh->sh_link, ELF32_R_SYM(reltab->r_info));
    }

    switch (ELF32_R_TYPE(reltab->r_info))
    {
    case R_386_NONE:
        break;
    case R_386_32:
        break;
    case R_386_PC32:
        break;
    default:
        break;
    }

}

void elf_load_phase_2( struct elf_file* elf, process_t* proc)
{
    for (size_t i = 0; i < elf->header->e_shnum; i++)
    {
        Elf32_Shdr* sh = elf_get_shdr( elf, i );
        if (sh->sh_type == SHT_REL)
        {
            for (size_t rel = 0; rel < sh->sh_size / sh->sh_entsize; rel++)
            {
                Elf32_Rel* reltab = &((Elf32_Rel*)((int)elf->header + sh->sh_offset))[rel];
                err_t e = elf_reloc(elf, reltab, sh);
                if (e != OS32_SUCCESS)
                {
                    __set_errno(EPERM);
                }
            }
        }
    }
}

elf_fn elf_load_for_exec( struct elf_file* elf, process_t* proc )
{
    elf_load_phase_1(elf, proc);
    elf_load_phase_2(elf, proc);

    for (size_t i = 0; i < elf->header->e_shnum; i++)
    {
        Elf32_Shdr* sh = elf_get_shdr( elf, i );
        switch (sh->sh_type)
        {
        case SHT_NULL:
        case SHT_NOBITS:
        case SHT_REL:
        case SHT_RELA:
            break;
        default:
            if ((sh->sh_flags & SHF_ALLOC) && sh->sh_entsize > 0)
            {
                page_table_ent_t perms = {0};
                perms.present = 1;
                perms.rw = (bool)(sh->sh_flags & SHF_WRITE);
                perms.user = 1;
                void* page_aligned_shaddr = (void*)(sh->sh_addr & (~0x03ff));
                
                size_t pgs = ((sh->sh_size + (sh->sh_addr - (uint32_t)page_aligned_shaddr) )  / PAGE_SIZE)+1;

                kmalloc_alloc_pages( 
                    proc->pdir, 
                    pgs, 
                    page_aligned_shaddr, 
                    perms
                );
                for (size_t i = 0; i < pgs; i++)
                {
                    void* addr = (void*)((uint32_t)page_aligned_shaddr + i * PAGE_SIZE);
                    wire_page( 
                        &boot_page_directory, 
                        phys_addr_of( proc->pdir, addr ),
                        addr,
                        (page_table_ent_t){.present=1,.rw=1} 
                    );
                    // memset( addr, 0, PAGE_SIZE);
                    memcpy( sh->sh_addr+i*PAGE_SIZE, (char*)elf->header+sh->sh_offset+i*PAGE_SIZE, PAGE_SIZE);
                    unwire_page( 
                        &boot_page_directory, 
                        addr
                    );
                }
            }
            break;
        }
    }

    return elf->header->e_entry;
}
elf_fn elf_get_sym( struct elf_file* elf, const char* symbol )
{

}
err_t elf_unload_from_exec( struct elf_file* elf, process_t* proc )
{

}
