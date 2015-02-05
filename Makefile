CFLAGS=-m32

C_SOURCES = $(wildcard kernel/*.c drivers/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h)

OBJ = ${C_SOURCES:.c=.o}

all: os.iso

run: all
		qemu-system-x86_64 -cdrom os.iso

bochs: all
		bochs

os.iso: os-image
		mkisofs -no-emul-boot -o os.iso -b os-image .

os-image: boot/boot_sect.bin kernel.bin
		cat $^ > os-image

kernel.bin: kernel/kernel_entry.o ${OBJ}
		ld -o $@ -melf_i386 -Ttext 0x1000 $^ --oformat binary

%.o: %.c ${HEADERS}
		gcc ${CFLAGS} -ffreestanding -c $< -o $@

%.o: %.asm
		nasm $< -f elf32 -o $@

boot/boot_sect.bin: boot/boot_sect.asm
		nasm $< -f bin -o $@

%.bin: %.o
		nasm $< -f bin -o $@

clean:
		rm -fr *.bin *.dis *.o os-image
		rm -fr kernel/*.o boot/*.bin drivers/*.o
		rm -fr *.iso
		rm -fr *~ boot/*~ kernel/*~
