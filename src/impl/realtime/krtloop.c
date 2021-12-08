#include "realtime/krtloop.h"
#include "system/process/multitasking.h"
void krtloop()
{
    __setpriority(PRI_MIN);

    while (true)
    {

        clean_processes(); // deal with finished processes

    }

}