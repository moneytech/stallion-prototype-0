.global stallion_syscall_exit

.section .text

stallion_syscall_exit:
  mov $0x01, %eax
  popl %ebx
  int $0x80
  pushl %eax
  ret