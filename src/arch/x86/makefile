libarch.a: a20.o boot.o gdt.o nmi.o
	$(AR) rvs libarch.a a20.o boot.o gdt.o nmi.o

a20.o: a20.asm
	$(ASM) $(ASMFLAGS) -o a20.o a20.asm

boot.o: boot.asm
	$(ASM) $(ASMFLAGS) -o boot.o boot.asm

gdt.o: gdt.asm
	$(ASM) $(ASMFLAGS) -o gdt.o gdt.asm

nmi.o: nmi.asm
	$(ASM) $(ASMFLAGS) -o nmi.o nmi.asm