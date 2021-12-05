#include "system/process/multitasking.h"
#include "boot/page.h"
#include "system/process/process.h"
#include "stdlib/instructions.h"
#include "stdlib/kmalloc.h"
#include "system/filesystems/vfs.h"
#include "stdlib/string.h"
#include "system/process/elf.h"
#include "drivers/timer.h"
// https://web.archive.org/web/20160326122214/http://jamesmolloy.co.uk/tutorial_html/9.-Multitasking.html
#define DUMMY_SWITCH 0x123

volatile process_t* current_process;
volatile process_t* process_list;
pid_t next_pid = 1;

multitasking_newproc_subroutine on_new_sub = NULL;

multitasking_newproc_subroutine 
register_on_newproc_subroutine(multitasking_newproc_subroutine newsub)
{
    multitasking_newproc_subroutine old = on_new_sub;
    on_new_sub = newsub;
    return old;
}

extern void* __bootstack_top;

void move_stack( void* dest, size_t size )
{
    kmalloc_alloc_pages( current_page_directory, size/PAGE_SIZE, dest-size, (page_table_ent_t){ .present=1, .rw=1, .user=0 } );
    uint32_t old_esp, old_ebp;
    uint32_t old_top = (uint32_t)&__bootstack_top;
    __get_esp(old_esp);
    __get_ebp(old_ebp);
    uint32_t offset = dest-old_top;
    uint32_t new_esp, new_ebp;
    new_esp = old_esp + offset;
    new_ebp = old_ebp + offset;
    memcpy( new_esp, old_esp, old_top - old_esp  );
    for (uint32_t* i = (uint32_t*)((uint32_t)dest-4); i > ((uint32_t)new_esp); i--)
    {
        if ( *i > old_esp && *i < old_top )
        {
            *i += offset;
        }
    }
    __set_esp(new_esp);
    __set_ebp(new_ebp);
}

err_t __install_multitasking()
{
    __cli
    current_process = (process_list = kmalloc( sizeof(process_t) ));
    process_create(current_process);
    current_process->pid = next_pid++;
    current_process->eip = __get_eip();
    uint32_t esp, ebp;
    __get_esp(esp);
    __get_ebp(ebp);
    current_process->esp = esp;
    current_process->ebp = ebp;
    
    set_pd(current_process->pdir);    
    move_stack( 0xf0000000, 16384 );
    

    __sti
    return OS32_SUCCESS;
}

void __procswitch()
{
    if (!current_process)
        return;
    uint32_t esp, ebp, eip;
    __get_esp(esp);
    __get_ebp(ebp);
    eip = __get_eip();
    if (eip == DUMMY_SWITCH)
    {
        return;
    }
    current_process->esp = esp;
    current_process->ebp = ebp;
    current_process->eip = eip;
    current_process = current_process->next;
    if (!current_process) current_process = process_list;    
    esp = current_process->esp;
    ebp = current_process->ebp;
    eip = current_process->eip;
    outportb(0x20, 0x20);

    // set_pd(current_process->pdir);
    asm volatile("         \
     cli;                 \
     mov %0, %%edi;       \
     mov %1, %%esp;       \
     mov %2, %%ebp;       \
     mov %3, %%cr3;       \
     mov $0x123, %%eax; \
     sti;                 \
     jmp *%%edi           "
                : : "r"(eip), "r"(esp), "r"(ebp), "r"(current_process->pdir->phys));
}
static void push_proc( process_t* proc )
{
    
    process_t* ptr = process_list;
    if (!ptr) process_list = proc;
    while (ptr->next)
    {
        ptr = ptr->next;
    }
    ptr->next = proc;
    if (on_new_sub)
    {
        on_new_sub(proc->pid);
    }
}

int __fork_sub(volatile process_t* parent, volatile process_t* newproc, uint32_t eip)
{
    if (current_process == parent)
    {
        uint32_t ebp, esp;
        __get_esp(esp);
        __get_ebp(ebp);
        newproc->ebp = ebp;
        newproc->esp = esp;
        newproc->eip = eip;
        __sti
        return newproc->pid;
    }
    else
    {
        return 0;
    }
}

int __fork()
{
    __cli
    volatile process_t* parent = current_process;
    volatile process_t* newproc = kmalloc( sizeof(process_t) );
    volatile uint32_t eip;
    process_create( newproc );
    newproc->pid = next_pid++;
    push_proc(newproc);
    dir_dup( newproc->pdir, parent->pdir );
    OS32_MAKEINSTR("nop;nop;nop;");
    eip = __get_eip();
    return __fork_sub(parent, newproc, eip);
    
}

int __getpid()
{
    return current_process->pid;
}

int __spawn(const char* path, const char** argv, const char* envp )
{
    pid_t pid = 0;
    if ((pid = __fork()) == 0)
    {
        __exec(path,argv,envp);
    }
    return pid;
}

err_t __exec( const char* path, const char** argv, const char* envp  )
{
    fd_t fd = vfs_open( path, 0);
    if (fd < 0)
    {
        return OS32_ERROR;
    }
    vfs_seekg( fd, 0, VFS_SEEK_END );
    size_t len = vfs_tellg(fd);
    vfs_seekg( fd, 0, VFS_SEEK_SET );
    char* data = kmalloc( len );
    if (!data || data == OS32_FAILED)
    {
        return OS32_ERROR;
    }
    err_t e = vfs_read(fd, data, len);
    if (e == OS32_ERROR)
    {
        return OS32_ERROR;
    }
    struct elf_file* elf = elf_load( data );
    if (elf == OS32_FAILED)
    {
        return OS32_ERROR;
    }
    process_t* proc = current_process;

    void (*entry)() = elf_load_for_exec( elf, proc );
    if (entry == OS32_FAILED)
    {
        return OS32_ERROR;
    }
    elf_free(elf);
    kfree(data);
    vfs_close(fd);
    process_start(proc, entry, argv, envp);

    process_destroy(proc);
    proc->eip = NULL;
    for(;;);
}

process_t* get_proc_by_id( pid_t pid )
{
    process_t* proc = process_list;
    if (!proc) return OS32_FAILED;
    if (proc->pid == pid) return proc;
    while (proc->next)
    {
        proc = proc->next;
        if (proc->pid == pid) return proc;
    }
    if (proc->pid == pid) return proc;
    return OS32_FAILED;

}

void __yield()
{
    current_process->quantum_progress = current_process->quantum;
    eflags_t flags;
    __get_eflags(flags);
    __sti
    pit_waitt(1);
    __set_eflags(flags);
}