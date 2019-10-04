export LIBC_DIR=$(shell pwd)/../libc
export CFLAGS+=-I$(shell pwd)/../libc/include \
	 -L$(LIBC_DIR) #-ffreestanding -nostdlib

all: sanity_check.mod

%.mod: %.o
	$(LD) -L$(LIBC_DIR) -o $@ $< -lc
	# $(LD) -L$(LIBC_DIR) --oformat binary -o $@ $< -lc
	# $(CC) $(CFLAGS) -o $@ $< -lc

# %.mod: %.s
# 	$(AS) $(ASFLAGS) -o $@ $<
