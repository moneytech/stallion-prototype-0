export COMPILER_PREFIX := i686-elf
export ARCH := x86
export ASM := nasm
export ASMFLAGS := -felf32
export AR := $(COMPILER_PREFIX)-ar
export CC := $(COMPILER_PREFIX)-gcc
export CFLAGS := --std=gnu99 -ffreestanding -O2 -Wall -Wextra
export QEMU := qemu-system-i386

.PHONY: all clean src

all: src run

clean:
	find . -type f -name '*.o' -delete
	find . -type f -name '*.iso' -delete
	find . -type f -name '*.a' -delete
	find . -type f -name '*.bin' -delete
	rm -rf src/build

src:
	$(MAKE) -C src

run: src/stallion.iso
	$(QEMU) -cdrom src/stallion.iso