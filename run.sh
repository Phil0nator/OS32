make OS32
qemu-system-i386 dist/x86/kernel.iso &> qemu_output.txt &
# gdb dist/x86/kernel.elf