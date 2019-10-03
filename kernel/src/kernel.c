#include <multiboot2.h>
#include <stallion.h>

void stallion_kernel_main(unsigned long magic, void *addr) {
  if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
    kputs("Invalid magic given; not Multiboot2. Aborting.");
    return;
  }

  kputs("Correct magic - Multiboot2 detected.");
}

