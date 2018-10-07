all: stallion.iso

stallion.bin:
	$(MAKE) -C arch/$(ARCH)
	$(MAKE) -C kernel
	$(CC) -T linker.ld -o stallion.bin -ffreestanding -O2 -nostdlib \
	arch/$(ARCH)/libarch.a \
	kernel/libkernel.a \
	-lgcc

verify: stallion.bin
	grub-file --is-x86-multiboot stallion.bin

stallion.iso: verify grub.cfg
	mkdir -p build/boot/grub
	cp stallion.bin build/boot/stallion.bin
	cp grub.cfg build/boot/grub/grub.cfg
	grub-mkrescue -o stallion.iso build

