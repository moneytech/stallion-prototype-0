export AR=$(TARGET)-ar
export AS=$(TARGET)-as
export CC=$(TARGET)-gcc
export LD=$(TARGET)-ld
export RANLIB=$(TARGET)-ranlib
export CFLAGS+=-std=gnu99 -ffreestanding -fno-builtin -nostdlib \
	-I$(shell pwd)/include \
	-I$(shell pwd)/../toolchain/newlib-copy/newlib/libc/sys/stallion/include
export PATH:="$(shell pwd)/build/bin:$(PATH)"
KERNEL=src/stallion.bin

.PHONY: $(KERNEL) $(LIBC_TARGETS) clean distclean

all: stallion.iso

clean:
	rm -rf .isodir

$(KERNEL):
	$(MAKE) -C src

stallion.iso: $(KERNEL) $(LIBC_TARGETS)
	mkdir -p .isodir/sys
	# cp core_modules/*.mod .isodir/sys
	cp -r iso_contents/* .isodir
	cp $< .isodir/boot/stallion.bin
	grub-mkrescue -o $@ .isodir