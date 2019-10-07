#include <multiboot2.h>
#include <stallion_process.h>

extern uint32_t startkernel;

void stallion_kernel_main(unsigned long magic, void *addr) {
  if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
    kputs("Invalid magic given; not Multiboot2. Aborting.");
    return;
  }

  stallion_t os;
  stallion_early_init(&os, magic, addr);

  // Gather all modules, and execute them.
  // Note that we haven't page-mapped these regions yet, so we'll have to
  // each time.

  struct multiboot_tag *tag = addr + 8; // Skip size, AND reserved u32 field.
  stallion_page_map(tag, tag, stallion_page_get_flag_kernel());

  // Find all modules, and load them as ELF binaries, in user mode.
  stallion_elf_binary_t *elf_modules = NULL;

  while (tag->type != MULTIBOOT_TAG_TYPE_END) {
    switch (tag->type) {
    case MULTIBOOT_TAG_TYPE_MODULE: {
      // We need to be able to read the memory, so page map it (read only).
      struct multiboot_tag_module *module = (struct multiboot_tag_module *)tag;
      void *p = (void *)module->mod_start;
      size_t sz = module->mod_end - module->mod_start;
      uint32_t flags = stallion_page_get_flag_user();
      stallion_page_map_region(p, p, sz, flags);

      // Load the ELF binary.
      const char *msg;
      // TODO: Handle OOM
      stallion_elf_binary_t *binary = stallion_elf_binary_create();
      if (!stallion_elf_read_binary(p, sz, binary, &msg)) {
        kputs(msg);
      }
      binary->next = elf_modules;
      elf_modules = binary;
    } break;
    }
    // Jump to the next one.
    struct multiboot_tag *old_tag = tag;
    tag = (struct multiboot_tag *)((multiboot_uint8_t *)tag +
                                   ((tag->size + 7) & ~7));
    stallion_page_unmap(old_tag);
    stallion_page_map(tag, tag, stallion_page_get_flag_kernel());
  }

  // Next, start each loaded module.
  // TODO: Pass command line
  stallion_elf_binary_t *module = elf_modules;
  while (module != NULL) {
    void *entry_point = (void *)module->header->entry_point;
    kputptr("Module entry", entry_point);
    typedef void (*Unsafe)();
    Unsafe unsafe = entry_point;
    unsafe();
    // stallion_enter_ring3(entry_point);
    module = module->next;
  }

  kputs("All modules loaded.");
}
