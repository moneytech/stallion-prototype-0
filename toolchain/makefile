export CFLAGS+=-D_COMPILING_NEWLIB
export PATH:="$(shell pwd)/build/bin:$(PATH)"
ACLOCAL=$(shell pwd)/build/bin/aclocal
AUTOCONF=$(shell pwd)/build/bin/autoconf
AUTOMAKE=$(shell pwd)/build/bin/automake
AUTORECONF=$(shell pwd)/build/bin/autoreconf
BUILD_DIR=$(shell pwd)/build
BUILD_BIN=$(BUILD_DIR)/bin
NEWLIB=$(BUILD_DIR)/usr/$(ARCH)-stallion/lib/libc.a
NEWLIB_DIR=$(BUILD_DIR)/newlib-src

.PHONY: clean distclean newlib-copy

all: $(NEWLIB)

clean:
	rm -rf $(NEWLIB_DIR)/newlib/libc/sys/configure \
	$(NEWLIB_DIR)/newlib/libc/sys/stallion/autom4te.cache \
	build/newlib/Makefile
	find . -name 'autom4te.cache' -delete

distclean: clean
	rm -rf build

$(NEWLIB): build/newlib/Makefile
	mkdir -p $(BUILD_DIR)/usr/$(ARCH)-stallion/include/stallion
	$(MAKE) -C build/newlib all
	$(MAKE) -C build/newlib install DESTDIR="$(BUILD_DIR)"

autotools: $(AUTOCONF) $(AUTOMAKE)

build/newlib/Makefile: autotools \
	$(NEWLIB_DIR)/newlib/libc/sys/configure \
	$(NEWLIB_DIR)/newlib/libc/sys/stallion/Makefile
	ln -sf `which $(TARGET)-ar` "$(BUILD_BIN)/$(ARCH)-stallion-ar"
	ln -sf `which $(TARGET)-as` "$(BUILD_BIN)/$(ARCH)-stallion-as"
	ln -sf `which $(TARGET)-gcc` "$(BUILD_BIN)/$(ARCH)-stallion-gcc"
	ln -sf `which $(TARGET)-gcc` "$(BUILD_BIN)/$(ARCH)-stallion-cc"
	ln -sf `which $(TARGET)-ranlib` "$(BUILD_BIN)/$(ARCH)-stallion-ranlib"
	mkdir -p build/newlib
	cd build/newlib && $(NEWLIB_DIR)/configure --prefix=/usr \
	--target=$(ARCH)-stallion

$(NEWLIB_DIR)/newlib/libc/sys/configure: $(AUTOCONF) \
	$(NEWLIB_DIR)/config.sub \
	$(NEWLIB_DIR)/newlib/configure.host
	cd $(NEWLIB_DIR)/newlib/libc/sys && $(AUTOCONF)

$(NEWLIB_DIR)/newlib/libc/sys/stallion/Makefile: $(AUTOCONF) \
	newlib-copy
	cd $(NEWLIB_DIR)/newlib/libc/sys && $(AUTOCONF)
	cd $(NEWLIB_DIR)/newlib/libc/sys/stallion && $(AUTORECONF)

newlib-copy:
	cp -rpfv newlib-copy/ $(NEWLIB_DIR)

$(AUTOCONF):
	$(MAKE) -i -C build/autoconf-src install

$(AUTOMAKE):
	$(MAKE) -i -C build/automake-src install