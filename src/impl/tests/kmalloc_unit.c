#include "stdlib/string.h"
#include "stdlib/kmalloc.h"
#include "boot/vga.h"
void __kmalloc_test()
{
    for (size_t i = 0 ; i < 256; i++)
    {
        char** test = kmalloc(sizeof(char*)*10);
        for (size_t j = 0; j < 10; j++)
        {
            test[j] = kmalloc(10);
            memcpy(test[j], "Hello", 6);
        }
        for (size_t j = 0; j < 10; j++)
        {
            // vgaPuts(test[j]);
            kfree(test[j]);
        }
        kfree(test);
    }

    kmalloc_defrag();

    char* t2 = kcalloc(40960);
    kfree(t2);

    kmalloc_defrag();

}
