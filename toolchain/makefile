export PATH:="$(shell pwd)/build/bin:$(PATH)"
ACLOCAL=$(shell pwd)/build/bin/aclocal
AUTOCONF=$(shell pwd)/build/bin/autoconf
AUTOMAKE=$(shell pwd)/build/bin/automake
AUTORECONF=$(shell pwd)/build/bin/autoreconf
BUILD_DIR=$(shell pwd)/build
BUILD_BIN=$(BUILD_DIR)/bin
NEWLIB=newlib
NEWLIB_DIR=$(BUILD_DIR)/newlib-src

.PHONY: clean distclean

all: build/newlib/Makefile #$(NEWLIB)

clean:
	rm -rf $(NEWLIB_DIR)/newlib/libc/sys/configure \
	$(NEWLIB_DIR)/newlib/libc/sys/stallion/autom4te.cache
	find . -name 'autom4te.cache' -delete

distclean: clean
	rm -rf build

$(NEWLIB): build/newlib/makefile
	# $(MAKE) -C build/newlib all

autotools: $(AUTOCONF) $(AUTOMAKE)

newlib-copy: # $(wildcard newlib-copy/*)
	cp -rpfv newlib-copy/ $(NEWLIB_DIR)

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

$(AUTOCONF):
	$(MAKE) -i -C build/autoconf-src install

$(AUTOMAKE):
	$(MAKE) -i -C build/automake-src install