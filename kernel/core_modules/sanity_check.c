#include <stallion/stallion.h>

void main() {
  stallion_syscall_declare_attributes(STALLION_ATTRIBUTE_FILESYSTEM);
  stallion_syscall_exit(0x23);
}