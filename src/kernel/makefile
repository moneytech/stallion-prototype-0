libkernel.a: kernel_main.o
	$(AR) rvs libkernel.a kernel_main.o

kernel_main.o: kernel_main.c
	$(CC) $(CFLAGS) -c -o kernel_main.o kernel_main.c