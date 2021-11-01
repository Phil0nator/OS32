
int bss;
int data = 1;
const char* str = "fancy string";

void testfn()
{
    for(;;);
}

int _start(int argc, char const *argv[])
{
    for(int i = 0; i < argc; i++)
    {
        argv[i];
    }
    testfn();
    return 0;
}
