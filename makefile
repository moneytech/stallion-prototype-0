export ARCH=i686-elf
export AR=$(ARCH)-ar
export AS=$(ARCH)-as
export CC=$(ARCH)-gcc
export CFLAGS+=-std=gnu99 -ffreestanding -fno-builtin -nostdlib
export DEBUGASFLAGS=-g
export DEBUGCFLAGS=-gdwarf-2 -g3 -DSTALLION_DEBUG=1
export QEMU=qemu-system-i386
KERNEL=kernel/stallion.iso

.PHONY: clean $(KERNEL)

all: $(KERNEL)

clean:
	find . \( -name '*.o' -o -name '*.a' \
		-o -name '*.bin' -o -name '*.iso' \) \
		-delete
	$(MAKE) -C kernel clean

$(KERNEL):
	$(MAKE) -C kernel

kernel: $(KERNEL)
kernel-debug: ASFLAGS+=$(DEBUGASFLAGS) CFLAGS+=$(DEBUGCFLAGS)
kernel-debug: kernel

bochs: kernel-debug debug.rc
	bochs -q -rc debug.rc

qemu: kernel-debug
	$(QEMU) -cdrom $(KERNEL) -serial file:/dev/stdout -no-reboot \
		-d int -no-shutdown -s -S

qemu-no-gdb: $(KERNEL)
	$(QEMU) -cdrom $< -no-reboot -no-shutdown # -serial file:/dev/stdout