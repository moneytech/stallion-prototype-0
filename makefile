export ARCH=i686
export TARGET=$(ARCH)-elf
export AR=$(TARGET)-ar
export AS=$(TARGET)-as
export CC=$(TARGET)-gcc
export LD=$(TARGET)-ld
export RANLIB=$(TARGET)-ranlib
export CFLAGS+=-std=gnu99 -ffreestanding -fno-builtin -nostdlib \
-DSTALLION_BITS_32
export DEBUGASFLAGS=-g
export DEBUGCFLAGS=-gdwarf-2 -g3 -DSTALLION_DEBUG=1
export QEMU=qemu-system-i386
KERNEL=kernel/stallion.iso

.PHONY: clean distclean $(KERNEL)

all: $(KERNEL)

debug: ASFLAGS+=$(DEBUGASFLAGS)
debug: CFLAGS+=$(DEBUGCFLAGS)
debug: all

distclean: clean
	$(MAKE) -C kernel distclean

clean:
	find . \( -name '*.o' -o -name '*.a' \
		-o -name '*.bin' -o -name '*.iso' -o -name '*.mod' \) \
		-delete
	$(MAKE) -C kernel clean

$(KERNEL):
	$(MAKE) -C kernel

kernel: $(KERNEL)

bochs: debug debug.rc
	bochs -q -rc debug.rc

qemu: debug
	$(QEMU) -cdrom $(KERNEL) -serial file:/dev/stdout -no-reboot \
		-d int -no-shutdown -s -S

qemu-no-gdb: debug
	$(QEMU) -cdrom $(KERNEL) -no-reboot -no-shutdown -serial file:/dev/stdout
