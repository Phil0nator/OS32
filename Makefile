CC = gcc
CFLAGS = -g -F dwarf -O0 -m32 -Isrc/include -c -ffreestanding -nostdlib -mno-red-zone -Wall -Wextra
USERMODEFLAGS = -O0 -m32 -nostdlib -static -ffreestanding -Wall -Wextra -Iuserlib/include


ASMSRC := $(shell find src/ -name "*.asm")
ASMOUT := $(patsubst src/%.asm, build/%.o, ${ASMSRC} )

CSRC := $(shell find src/ -name "*.c")
COUT := $(patsubst src/%.c, build/%.o, ${CSRC})

USRSRC := $(shell find usermode/ -name "*.c")
USROUT := $(patsubst usermode/%.c, build/bin/%, ${USRSRC})

USERLIB: $(shell find userlib/src/ -name "*.c")
	cd userlib && $(MAKE) && cd ..

$(ASMOUT): build/%.o : src/%.asm
	mkdir -p $(dir $@) && \
	nasm -f elf32 -F dwarf -g $(patsubst build/%.o, src/%.asm, $@) -o $@

$(COUT): build/%.o : src/%.c
	mkdir -p $(dir $@) && \
	${CC} ${CFLAGS} $(patsubst build/%.o, src/%.c, $@) -o $@

$(USROUT): build/bin/% : usermode/%.c USERLIB
	mkdir -p $(dir $@) && \
	${CC} userlib/build/userlib.a ${USERMODEFLAGS} ${patsubst build/bin/%, usermode/%.c, $@} -o $@

OS32: $(COUT) $(ASMOUT) $(USROUT)
	mkdir -p dist/x86 && \
	cp -r build/bin/ /media/philokaulkin/a9ba968b-47d9-4a0f-bf54-f494689dc8d9 &&\
	ld -O3 -melf_i386 -n -o targets/x86/iso/boot/kernel.elf -T targets/x86/linker.ld $(ASMOUT) $(COUT) && \
	cp targets/x86/iso/boot/kernel.elf dist/x86/kernel.elf && \
	grub-mkrescue /usr/lib/grub/i386-pc -o dist/x86/kernel.iso targets/x86/iso

OS32_DBG: OS32
	sleep 1 && qemu-system-i386 dist/x86/kernel.iso -s -S

purge:
	rm -rf build/ && mkdir build



.PHONY=OS32
.PHONY=OS32_DBG
.DEFAULT=OS32