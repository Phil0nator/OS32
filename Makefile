CC = gcc
CFLAGS = -g -F dwarf -Os -m32 -Isrc/include -c -ffreestanding -nostdlib -mno-red-zone -Wall -Wextra

ASMSRC := $(shell find src/ -name "*.asm")
ASMOUT := $(patsubst src/%.asm, build/%.o, ${ASMSRC} )

CSRC := $(shell find src/ -name "*.c")
COUT := $(patsubst src/%.c, build/%.o, ${CSRC})

$(ASMOUT): build/%.o : src/%.asm
	mkdir -p $(dir $@) && \
	nasm -f elf32 -F dwarf -g $(patsubst build/%.o, src/%.asm, $@) -o $@

$(COUT): build/%.o : src/%.c
	mkdir -p $(dir $@) && \
	${CC} ${CFLAGS} $(patsubst build/%.o, src/%.c, $@) -o $@

OS32: $(COUT) $(ASMOUT)
	mkdir -p dist/x86 && \
	ld -O3 -melf_i386 -n -o targets/x86/iso/boot/kernel.elf -T targets/x86/linker.ld $(ASMOUT) $(COUT) && \
	cp targets/x86/iso/boot/kernel.elf dist/x86/kernel.elf && \
	grub-mkrescue /usr/lib/grub/i386-pc -o dist/x86/kernel.iso targets/x86/iso

OS32_DBG: OS32
	qemu-system-i386 dist/x86/kernel.iso -s -S

purge:
	rm -rf build/ && mkdir build

.PHONY=OS32
.PHONY=OS32_DBG
.DEFAULT=OS32