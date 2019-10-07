export CFLAGS+=-I$(shell pwd)/include
LIBARCH=$(TARGET)/libarch.a

.PHONY: $(LIBARCH)

all: stallion.bin

$(LIBARCH):
	$(MAKE) -C $(TARGET)

stallion.bin: scheduler.o elf.o kernel.o init.o liballoc.o util.o $(LIBARCH)
	$(CC) $(CFLAGS) -T linker.ld -o $@ $^
	grub-file --is-x86-multiboot2 $@