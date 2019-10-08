export ARCH=i686
export TARGET=$(ARCH)-elf
export CFLAGS+=-DSTALLION_BITS_32
export DEBUGASFLAGS=-g
export DEBUGCFLAGS=-gdwarf-2 -g3 -DSTALLION_DEBUG=1
export QEMU=qemu-system-i386
KERNEL=kernel/src/stallion.bin
ISO=stallion.iso

.PHONY: clean distclean toolchain $(KERNEL)

all: $(ISO)

debug: ASFLAGS+=$(DEBUGASFLAGS)
debug: CFLAGS+=$(DEBUGCFLAGS)
debug: all

distclean: clean
	$(MAKE) -C toolchain distclean

clean:
	find . \( -name '*.o' -o -name '*.a' \
		-o -name '*.bin' -o -name '*.iso' -o -name '*.mod' \) \
		-delete
	rm -rf .isodir
	$(MAKE) -C kernel clean
	$(MAKE) -C toolchain clean

$(KERNEL):
	$(MAKE) -C kernel

$(ISO): $(KERNEL)
	mkdir -p .isodir/sys
	# cp core_modules/*.mod .isodir/sys
	cp -r iso_contents/* .isodir
	cp $< .isodir/boot/stallion.bin
	grub-mkrescue -o $@ .isodir

kernel: $(KERNEL)

toolchain:
	$(MAKE) -C toolchain

bochs: debug debug.rc
	bochs -q -rc debug.rc

qemu: debug
	$(QEMU) -cdrom $(ISO) -serial file:/dev/stdout -no-reboot \
		-d int -no-shutdown -s -S

qemu-no-gdb: debug
	$(QEMU) -cdrom $(ISO) -no-reboot -no-shutdown -serial file:/dev/stdout
