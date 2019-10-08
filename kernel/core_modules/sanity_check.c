#include <stallion/stallion.h>
#include <stdint.h>

void main() {
  uint32_t pid = stallion_syscall_get_pid();
  stallion_syscall_exit(pid);
  // stallion_syscall_declare_attributes(STALLION_ATTRIBUTE_FILESYSTEM);
  // stallion_syscall_exit(0x23);
}