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

.PHONY: $(KERNEL) clean distclean

all: $(KERNEL)

clean:

$(KERNEL):
	$(MAKE) -C src