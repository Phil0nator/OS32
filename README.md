# OS32

This is a 32bit x86 hobby kernel I have been putting together with help from [osdev](https://wiki.osdev.org/Main_Page), [Bran's Kernel Tutorial](http://www.osdever.net/bkerndev/Docs/title.htm), other tutorials and resources, and some extra tips from [Zack Mayhew](https://github.com/zacklukem).

## Dependencies
- Grub
- Multiboot
- Xorriso
- gcc/ld/nasm for i386

## Progress
- [ OK ] Boot
- [ OK ] Multiboot interpretation
- [ OK ] GDT setup
- [ OK ] IDT setup
- [ OK ] ISRs setup
- [ OK ] IRQs setup
- [ OK ] basic TSS setup
- [ OK ] basic PIT drivers
- [ OK ] basic Keyboard drivers
- [ OK ] kmalloc and paging
- [ OK ] FPU setup
- [ OK ] INITRD loading

## Still WIP
- [...] ext2 fs implimentation
- [...] Usermode programs
- [...] syscalls
- [...] ...