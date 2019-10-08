export CFLAGS+=-I$(shell pwd)/include -I$(shell pwd)/libc/include
export PATH:="$(shell pwd)/build/bin:$(PATH)"
ACLOCAL=$(shell pwd)/build/bin/aclocal
AUTOCONF=$(shell pwd)/build/bin/autoconf
AUTOMAKE=$(shell pwd)/build/bin/automake
AUTORECONF=$(shell pwd)/build/bin/autoreconf
BUILD_DIR=$(shell pwd)/build
BUILD_BIN=$(BUILD_DIR)/bin
KERNEL=src/stallion.bin
LIBC_TARGETS=libc/libc.a src/libm.a
NEWLIB=newlib
NEWLIB_DIR=newlib-cygwin

.PHONY: $(KERNEL) $(LIBC_TARGETS) clean core_modules distclean libc \
	autotools $(NEWLIB)

all: stallion.iso autotools $(NEWLIB)

clean:
	rm -rf .isodir $(NEWLIB_DIR)/newlib/libc/sys/configure \
	$(NEWLIB_DIR)/newlib/libc/sys/stallion/autom4te.cache
	find . -name 'autom4te.cache' -delete

distclean: clean
	rm -rf build

$(KERNEL):
	$(MAKE) -C src

$(NEWLIB): build/newlib/makefile
	# $(MAKE) -C build/newlib all

build/newlib/makefile: autotools \
	aclocal_m4 \
	$(NEWLIB_DIR)/newlib/libc/sys/configure \
	$(NEWLIB_DIR)/newlib/libc/sys/stallion/makefile
	ln -sf `which $(AR)` "$(BUILD_BIN)/$(ARCH)-stallion-ar"
	ln -sf `which $(AS)` "$(BUILD_BIN)/$(ARCH)-stallion-as"
	ln -sf `which $(CC)` "$(BUILD_BIN)/$(ARCH)-stallion-gcc"
	ln -sf `which $(CC)` "$(BUILD_BIN)/$(ARCH)-stallion-cc"
	ln -sf `which $(RANLIB)` "$(BUILD_BIN)/$(ARCH)-stallion-ranlib"
	mkdir -p build/newlib
	cd build/newlib && ../../$(NEWLIB_DIR)/configure --prefix=/usr \
	--target=$(ARCH)-stallion

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