#define __ASSEMBLY__
#include "stallion/stallion.h"

.global stallion_syscall_exit
.global stallion_syscall_declare_attributes
.global stallion_syscall_request_permissions

.section .text

stallion_syscall_exit:
  mov 4(%esp), %ebx
  mov $STALLION_SYSCALL_EXIT, %eax
  int $0x80
  pushl %eax
  ret

stallion_syscall_declare_attributes:
  mov 4(%esp), %ebx
  mov $STALLION_SYSCALL_DECLARE_ATTRIBUTES, %eax
  int $0x80
  pushl %eax
  ret

stallion_syscall_request_permissions:
  mov 4(%esp), %ebx
  mov $STALLION_SYSCALL_REQUEST_PERMISSIONS, %eax
  int $0x80
  pushl %eax
  ret
