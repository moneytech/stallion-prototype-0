MBALIGN  equ  1 << 0
MEMINFO  equ  1 << 1
FLAGS    equ  MBALIGN | MEMINFO
MAGIC    equ  0x1BADB002
CHECKSUM equ -(MAGIC + FLAGS)

section .multiboot
align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

section .bss
    align 16
    stack_bottom:
        resb 16384
    stack_top:

section .text
global _start:function (_start.end - _start)
_start:
	; The bootloader has loaded us into 32-bit protected mode on a x86
	; machine. Interrupts are disabled. Paging is disabled. The processor
	; state is as defined in the multiboot standard. The kernel has full
	; control of the CPU. The kernel can only make use of hardware features
	; and any code it provides as part of itself. There's no printf
	; function, unless the kernel provides its own <stdio.h> header and a
	; printf implementation. There are no security restrictions, no
	; safeguards, no debugging mechanisms, only what the kernel provides
	; itself. It has absolute and complete power over the
	; machine.

	; To set up a stack, we set the esp register to point to the top of our
	; stack (as it grows downwards on x86 systems). This is necessarily done
	; in assembly as languages such as C cannot function without a stack.
	mov esp, stack_top

	; This is a good place to initialize crucial processor state before the
	; high-level kernel is entered. It's best to minimize the early
	; environment where crucial features are offline. Note that the
	; processor is not fully initialized yet: Features such as floating
	; point instructions and instruction set extensions are not initialized
	; yet. The GDT should be loaded here. Paging should be enabled here.
	; C++ features such as global constructors and exceptions will require
	; runtime support to work as well.
	extern disable_nmi
	extern enable_a20
	extern gdt

	call disable_nmi
	call enable_a20

    ; Enter protected mode.
	cli
	lgdt [gdt]
	mov eax, cr0
    or al, 1       ; set PE (Protection Enable) bit in CR0 (Control Register 0)
    mov cr0, eax

    ; Perform far jump to selector 08h (offset into GDT, pointing at a 32bit PM code segment descriptor)
    ; to load CS with proper PM32 descriptor)
    jmp 0x08:pmode_main
.end:

pmode_main:
    ; Now, we want to load the data segment (0x10)
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Stack
    mov ebp, 0x90000
    mov esp, ebp


	; Enter the high-level kernel. The ABI requires the stack is 16-byte
	; aligned at the time of the call instruction (which afterwards pushes
	; the return pointer of size 4 bytes). The stack was originally 16-byte
	; aligned above and we've since pushed a multiple of 16 bytes to the
	; stack since (pushed 0 bytes so far) and the alignment is thus
	; preserved and the call is well defined.
        ; note, that if you are building on Windows, C functions may have "_" prefix in assembly: _kernel_main
	extern kernel_main
	call kernel_main

	; If the system has nothing more to do, put the computer into an
	; infinite loop. To do that:
	; 1) Disable interrupts with cli (clear interrupt enable in eflags).
	;    They are already disabled by the bootloader, so this is not needed.
	;    Mind that you might later enable interrupts and return from
	;    kernel_main (which is sort of nonsensical to do).
	; 2) Wait for the next interrupt to arrive with hlt (halt instruction).
	;    Since they are disabled, this will lock up the computer.
	; 3) Jump to the hlt instruction if it ever wakes up due to a
	;    non-maskable interrupt occurring or due to system management mode.
	cli
.hang:	hlt
	jmp .hang