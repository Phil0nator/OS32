CC = gcc
USERMODEFLAGS = -Iinclude -I../src/include -O0 -g -m32 -nostdlib -static -ffreestanding -Wall -Wextra

SRCS := $(shell find src -name "*.c")
OUTS := $(patsubst src/%.c, build/%.o, ${SRCS})



build/userlib.a: $(OUTS)
	ld -O3 -melf_i386 -r ${OUTS} -o build/userlib.a

$(OUTS): build/%.o : src/%.c
	mkdir -p $(dir $@) &&\
	${CC} ${USERMODEFLAGS} -c $(patsubst build/%.o, src/%.c, $@) -o $@

purge:
	rm -rf build/ && mkdir build