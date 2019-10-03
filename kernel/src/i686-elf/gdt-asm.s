.global stallion_gdt_enable
.section .text
stallion_gdt_enable:
  sti
  popl %eax
  lgdt (%eax)
  jmp $0x08, $stallion_gdt_enable_done
stallion_gdt_enable_done:
  mov $0x10, %ax
  mov %ax, %ds
  mov %ax, %es
  mov %ax, %fs
  mov %ax, %gs
  mov %ax, %ss
  cli
  ret