#include <multiboot2.h>
#include <stallion.h>
#include <stallion_elf.h>

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

  // Find all modules, and load them as ELF binaries, in user mode.
  while (tag->type != MULTIBOOT_TAG_TYPE_END) {
    switch (tag->type) {
    case MULTIBOOT_TAG_TYPE_MODULE: {
      // We need to be able to read the memory, so page map it (read only).
      struct multiboot_tag_module *module = (struct multiboot_tag_module *)tag;
      void *p = (void *)module->mod_start;
      size_t sz = module->mod_end - module->mod_start;
      uint32_t flags = stallion_page_get_flag_kernel();
      stallion_page_map_region(p, p, sz, flags);

      // Load the ELF binary.
      const char *msg;
      stallion_elf_binary_t *binary = stallion_elf_binary_create();
      if (!stallion_elf_read_binary(p, sz, binary, &msg)) {
        kputs(msg);
      } else {
        kputs("Loaded ELF binary.");
      }
    } break;
    }
    // Jump to the next one.
    struct multiboot_tag *old_tag = tag;
    tag = (struct multiboot_tag *)((multiboot_uint8_t *)tag +
                                   ((tag->size + 7) & ~7));
    stallion_page_unmap(old_tag);
    stallion_page_map(tag, tag, stallion_page_get_flag_kernel());
  }

  kputs("All modules loaded.");
}
