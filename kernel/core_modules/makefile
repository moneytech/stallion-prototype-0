export CFLAGS+=-I$(shell pwd)/../libc/include \
	 -L$(shell pwd)/../libc #-ffreestanding -nostdlib

all: sanity_check.mod

%.mod: %.o
	$(LD) -o $@ $<
	# $(CC) $(CFLAGS) -o $@ $< -lc

# %.mod: %.s
# 	$(AS) $(ASFLAGS) -o $@ $<