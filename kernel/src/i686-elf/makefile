export CFLAGS+=-I$(shell pwd)/include

libarch.a: boot.o early_init.o gdt.o gdt-asm.o \
	idt.o idt-asm.o interrupt.o paging.o paging-asm.o \
	serial_print.o
	$(AR) rcs $@ $^