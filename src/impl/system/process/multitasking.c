#include "system/process/multitasking.h"
#include "boot/page.h"
#include "system/process/process.h"
#include "stdlib/instructions.h"
#include "stdlib/kmalloc.h"
#include "system/filesystems/vfs.h"
// https://web.archive.org/web/20160326122214/http://jamesmolloy.co.uk/tutorial_html/9.-Multitasking.html
#define DUMMY_SWITCH 0x123

process_t* current_process;
process_t* process_list;
pid_t next_pid = 1;


void move_stack( void* dest, size_t size )
{
    kmalloc_alloc_pages( current_page_directory, size/PAGE_SIZE, dest, (page_table_ent_t){ .present=1, .rw=1, .user=0 } );
    
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
    set_pd(current_process->pdir);
    asm volatile("         \
     cli;                 \
     mov %0, %%edi;       \
     mov %1, %%esp;       \
     mov %2, %%ebp;       \
     mov $0x123, %%eax; \
     sti;                 \
     jmp *%%edi           "
                : : "r"(eip), "r"(esp), "r"(ebp));
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
}

int __fork()
{
    __cli
    process_t* parent = current_process;
    process_t* newproc = kmalloc( sizeof(process_t) );
    process_create( newproc );
    newproc->pid = next_pid++;
    dir_dup( newproc->pdir, parent->pdir );
    push_proc(newproc);
    uint32_t eip = __get_eip();
    

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
        __sti
        return 0;
    }
}

int __getpid()
{
    return current_process->pid;
}

int __spawn(const char* path)
{
    pid_t pid = 0;
    if ((__fork() == 0))
    {
        fd_t fd = vfs_open( path, 0);
        if (fd < 0)
        {
            kpanic("could not open test");
        }
        vfs_seekg( fd, 0, VFS_SEEK_END );
        size_t len = vfs_tellg(fd);
        vfs_seekg( fd, 0, VFS_SEEK_SET );
        char* data = kmalloc( len );
        vfs_read(fd, data, len);
        struct elf_file* elf = elf_load( data );
        process_t* proc = current_process;

        void (*entry)() = elf_load_for_exec( elf, proc );
        elf_free(elf);
        kfree(data);
        vfs_close(fd);
        process_start(proc, entry);

        process_destroy(proc);
        proc->eip = NULL;
        for(;;);

    }
    return pid;
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