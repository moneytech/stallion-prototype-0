.set MAGIC, 0xe85250d6
.set ARCH, 0
.set HDRLEN, (multiboot_end - multiboot_start)
 
# Multiboot2
.section .multiboot2
.align 8
multiboot_start:
.long MAGIC
.long ARCH
.long HDRLEN  # Header length
# Checksum
.long -(MAGIC + ARCH + HDRLEN)
# .long -(MULTIBOOT2_HEADER_MAGIC + GRUB_MULTIBOOT_ARCHITECTURE_I386 + HDRLEN)

# Get the address of the multiboot2 header.
# addr_tag_start:      
# 	.short MULTIBOOT_HEADER_TAG_ADDRESS
# 	.short MULTIBOOT_HEADER_TAG_OPTIONAL
# 	.long addr_tag_end - addr_tag_start
# 	/*  header_addr */
# 	.long   multiboot_start
# 	/*  load_addr */
# 	.long   _start
# 	/*  load_end_addr */
# 	.long   _edata
# 	/*  bss_end_addr */
# 	.long   _ebss
# addr_tag_end:

# Request the memory map.

# TODO: Request the boot command line.

# Close off the tag list.
.short 0
.short 0
.long 8
multiboot_end:
 
# Make a litle stack.
.section .bss
.align 16
stack_bottom:
.skip 16384
stack_top:
 
# Entry
.section .text
.global _start
.type _start, @function
_start:
	# There is stuff to do before entering the kernel.
	mov $stack_top, %esp
	# Reset EFlags.
	push $0
	popf

	# Push the Multiboot info struct, and the magic.
	push %ebx
	push %eax

	# Kernel!
	call kernel_main
	# Forever loop.
	cli
1:	hlt
	jmp 1b
 
/*
Set the size of the _start symbol to the current location '.' minus its start.
This is useful when debugging or when you implement call tracing.
*/
.size _start, . - _start
