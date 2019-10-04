#include <stallion_i686_elf.h>
#include <stallion_process.h>

stallion_tss_entry_t stallion_tss;

void stallion_enter_ring3(void *entry_point) {
  // Save ESP, for when we come back from ring3.
  uint32_t esp;
  asm volatile("mov %%esp, %0" : "=a"(esp));
  stallion_tss.esp0 = esp;
  stallion_tss_flush((uint32_t)entry_point);
}