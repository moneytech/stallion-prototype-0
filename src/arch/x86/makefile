libarch.a: a20.o boot.o create_page_directory.o gdt.o nmi.o paging.o
	$(AR) rvs libarch.a a20.o boot.o create_page_directory.o gdt.o nmi.o paging.o

a20.o: a20.asm
	$(ASM) $(ASMFLAGS) -o a20.o a20.asm

boot.o: boot.asm
	$(ASM) $(ASMFLAGS) -o boot.o boot.asm

create_page_directory.o: create_page_directory.c
	$(CC) $(CFLAGS) -c -o create_page_directory.o create_page_directory.c

gdt.o: gdt.asm
	$(ASM) $(ASMFLAGS) -o gdt.o gdt.asm

nmi.o: nmi.asm
	$(ASM) $(ASMFLAGS) -o nmi.o nmi.asm

paging.o: paging.asm
	$(ASM) $(ASMFLAGS) -o paging.o paging.asm