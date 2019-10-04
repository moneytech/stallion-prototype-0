export CFLAGS=-I$(shell pwd)/include
LIBC_OBJECTS=string.o syscall.s

all: libc.a libm.a crt0.o

libm.a: empty.o
	$(AR) rcs $@ $^

libc.a:	include/assert.h include/string.h \
	$(LIBC_OBJECTS)
	$(AR) rcs $@ $(LIBC_OBJECTS)