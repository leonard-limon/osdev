

CC = i386-elf-gcc
LINK = i386-elf-ld
CFLAGS = -ffreestanding -O3 -I . -std=c99

#-fno-optimize-sibling-calls

C_SRCS = $(wildcard kernel/*.c drivers/*.c mem/*.c cpu/*.c stdlib/*.c)
HEAD = ${C_SRCS:.c=.h}
ASM_SRCS = $(wildcard kernel/*.asm drivers/*.asm mem/*.asm cpu/*.asm stdlib/*.asm)

OBJ = ${C_SRCS:.c=.o}
ASM_OBJ = ${ASM_SRCS:.asm=.o}

os-image.img: boot/bootloader.img kern
	cat $^ /dev/zero | dd of=$@ bs=512 count=100

boot/bootloader.img:
	make -C boot

kern: kernel/kentry.o ${ASM_OBJ} ${OBJ}
	${LINK} -o $@ -Ttext 0x1000 $^ --oformat binary

run: os-image.img
	qemu-system-i386 -hda $<

debug: os-image.img
	qemu-system-i386 -hda $< -serial telnet:127.0.0.1:4444,server,nowait

%.o: %.asm
	nasm $< -f elf -o $@

%.o: %.c ${HEAD}
	${CC} ${CFLAGS} -c $< -o $@

clean:
	@rm ${OBJ} ${ASM_OBJ} os-image.img kern
