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

  // Gather all modules, and execute them.
  // Note that we haven't page-mapped these regions yet, so we'll have to
  // each time.

  struct multiboot_tag *tag = addr + 8; // Skip size, AND reserved u32 field.
  stallion_page_map(tag, tag, stallion_page_get_flag_kernel());

  while (tag->type != MULTIBOOT_TAG_TYPE_END) {
    switch (tag->type) {
    case MULTIBOOT_TAG_TYPE_MODULE: {
      struct multiboot_tag_module *module = (struct multiboot_tag_module *)tag;
    } break;
    }
    // Jump to the next one.
    tag = (struct multiboot_tag *)((multiboot_uint8_t *)tag +
                                   ((tag->size + 7) & ~7));
    stallion_page_map(tag, tag, stallion_page_get_flag_kernel());
  }

  kputs("All modules loaded.");
}
