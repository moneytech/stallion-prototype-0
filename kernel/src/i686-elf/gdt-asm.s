.section .text
.global stallion_gdt_enable
.type stallion_gdt_enable, @function
stallion_gdt_enable:
  mov 4(%esp), %eax
  lgdt (%eax)
  
  mov $0x10, %ax
  mov %ax, %ds
  mov %ax, %es
  mov %ax, %fs
  mov %ax, %gs
  mov %ax, %ss
  ljmp $0x08, $stallion_gdt_enable_done
  ; popl %eax
  ; ret
stallion_gdt_enable_done:
  ; sti
  ret