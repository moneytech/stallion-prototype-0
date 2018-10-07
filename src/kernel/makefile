libkernel.a: kernel.o
	$(AR) rvs libkernel.a kernel.o

kernel.o: kernel.c
	$(CC) $(CFLAGS) -c -o kernel.o kernel.c