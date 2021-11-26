#include "system/process/multitasking.h"
#include "boot/page.h"
#include "system/process/process.h"
#include "stdlib/instructions.h"
#include "stdlib/kmalloc.h"

#define DUMMY_SWITCH 0x123

process_t* current_process;
process_t* process_list;
pid_t next_pid = 1;

err_t __install_multitasking()
{
    __cli
    current_process = process_list = kmalloc( sizeof(process_t) );
    process_create(current_process);
    current_process->pid = next_pid++;
    __sti
    return OS32_SUCCESS;
}

void __procswitch()
{
    if (!current_process)
        return;
    uint32_t esp, ebp, eip;
    eip = __get_eip();
    __get_esp(esp);
    __get_ebp(ebp);
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
    __cli
    __set_esp( esp );
    __set_ebp( ebp );
    set_cr3( current_process->pdir_phys );
    __sti
    __long_jump( current_process->eip, "ecx" );
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
    

    if (current_process = parent)
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

int __getpid()
{
    return current_process->pid;
}