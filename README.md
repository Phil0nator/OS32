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
- [ OK ] basic RTC drivers
- [ OK ] basic Keyboard drivers
- [ OK ] kmalloc and paging
- [ OK ] FPU setup
- [ OK ] INITRD loading
- [ OK ] basic ext2 read-only operations

## Still WIP
- [...] ext2 write implimentation
- [...] full vfs implimentation
- [...] Usermode programs / ELF loading
- [...] syscalls
- [...] Multiprocessing
- [...] Terminal emulation / ANSI escape codes
- [...] Networking
- [...] ...