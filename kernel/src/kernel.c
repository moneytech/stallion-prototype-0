#include <multiboot2.h>
#include <stallion.h>

extern uint32_t startkernel;

void stallion_kernel_main(unsigned long magic, void *addr) {
  if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
    kputs("Invalid magic given; not Multiboot2. Aborting.");
    return;
  }

  stallion_t os;

  kputs("Correct magic - Multiboot2 detected.");
  stallion_early_init(&os, magic, addr);

  char *big_str = (char *)kmalloc(32768);
  kmemset(big_str, 0, 32768);
  kmemset(big_str, 'A', 70);
  kputs(big_str);
}
