export CFLAGS+=-I$(shell pwd)/include -I$(shell pwd)/libc/include
KERNEL=src/stallion.bin
LIBC_TARGETS=libc/libc.a src/libm.a

.PHONY: $(KERNEL) $(LIBC_TARGETS) clean core_modules distclean libc

all: stallion.iso

clean:
	rm -rf .isodir

distclean: clean
	rm -rf build

$(KERNEL):
	$(MAKE) -C src

libc: $(LIBC_TARGETS)

$(LIBC_TARGETS):
	$(MAKE) -C libc

core_modules: libc
	$(MAKE) -C core_modules

stallion.iso: $(KERNEL) $(LIBC_TARGETS) core_modules
	mkdir -p .isodir/sys
	cp core_modules/*.mod .isodir/sys
	cp -r iso_contents/* .isodir
	cp $< .isodir/boot/stallion.bin
	grub-mkrescue -o $@ .isodir