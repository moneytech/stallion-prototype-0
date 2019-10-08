export CFLAGS+=-I$(shell pwd)/include -I$(shell pwd)/libc/include
AUTOCONF=$(shell pwd)/build/bin/autoconf
AUTOMAKE=$(shell pwd)/build/bin/automake
KERNEL=src/stallion.bin
LIBC_TARGETS=libc/libc.a src/libm.a

.PHONY: $(KERNEL) $(LIBC_TARGETS) clean core_modules distclean libc \
	autotools

all: stallion.iso autotools

clean:
	rm -rf .isodir

distclean: clean
	rm -rf build

$(KERNEL):
	$(MAKE) -C src

autotools: $(AUTOCONF) $(AUTOMAKE)

$(AUTOCONF):
	$(MAKE) -C build/autoconf-src install

$(AUTOMAKE):
	$(MAKE) -C build/automake-src install

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