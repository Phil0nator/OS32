#include "realtime/krtloop.h"
#include "system/process/multitasking.h"
#include "stdlib/kmalloc.h"
void krtloop()
{
    // this thread is just general maintenance
    // so it doesn't need a huge timeslice
    __setpriority(PRI_MIN); 

    while (true)
    {

        clean_processes(); // deal with finished processes
        // kmalloc_defrag();  // defragment the kernel's heap

    }

}