#include <stallion.h>
#include <stallion_i686_elf.h>

void stallion_early_init(stallion_t *os, unsigned long magic, void *addr) {
  stallion_init_gdt(os);
  stallion_init_idt(os);
  stallion_init_paging(os);
}