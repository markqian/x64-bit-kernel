CFLAGS=-m64
C_SOURCES = $(wildcard kernel/*.c drivers/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h)
ASM_SOURCES = $(wildcard boot/*.asm)
OBJ = ${C_SOURCES:.c=.o}

all: os.iso

run: all
		qemu-system-x86_64 -cdrom os.iso

bochs: all
		bochs

os.iso: os-image
		mkisofs -no-emul-boot -o os.iso -b os-image cdcontent

os-image: boot/boot_sect.bin kernel.bin
		cat $^ > ./cdcontent/os-image

kernel.bin: kernel/kernel_entry.o ${OBJ} kernel/kprint_hex.o kernel/descriptor_load.o kernel/interrupt.o
		x86_64-elf-gcc -ffreestanding -z max-page-size=0x1000 -o $@ -T ./kernel/linker.ld $^ -nostdlib -lgcc 

%.o: %.c ${HEADERS}
		x86_64-elf-gcc -ffreestanding -mcmodel=large -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -c $< -o $@ 
		x86_64-elf-gcc -S -ffreestanding -mcmodel=large -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -c $< 

%.o: %.asm 
		nasm $< -f elf64 -o $@

kernel/descriptor_load.o: kernel/descriptor_load.asm
		nasm $< -f elf64 -o $@

kernel/kprint_hex.o: kernel/kprint_hex.asm
		nasm $< -f elf64 -o $@

kernel/interrupt.o: kernel/interrupt.asm
		nasm $< -f elf64 -o $@

boot/boot_sect.bin: boot/boot_sect.asm
		nasm $< -f bin -o $@

%.bin: %.o
		nasm $< -f bin -o $@

clean:
		rm -fr *.bin *.dis *.o os-image
		rm -fr kernel/*.o boot/*.bin drivers/*.o
		rm -fr *.iso
		rm -fr *~ boot/*~ kernel/*~
		rm -fr *.s
