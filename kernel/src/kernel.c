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
      // TODO: Handle failure to map region for module
      // kputs("Module region map failed");

      stallion_elf_header_t *header;
      if (!stallion_elf_read_header(p, sz, &header)) {
        // TODO: Handle ELF read failure.
        kputs("ELF read failure");
      } else {
        const char *msg;
        if (!stallion_elf_check_supported(header, &msg)) {
          // TODO: Properly handle ELF failures here?
          kputs(msg);
        } else {
          kputs("ELF read/supported success");

          // Read the headers...
          stallion_elf_section_header_t *section_headers =
              stallion_elf_get_section_header_array(header);

          for (uint16_t i = 0; i < header->section_header_entry_count; i++) {
            stallion_elf_section_header_t section_header = section_headers[i];
            if (section_header.type == STALLION_ELF_SECTION_SYMBOL_TABLE) {
              kputs("Symbol table");
            } else if (section_header.type ==
                       STALLION_ELF_SECTION_STRING_TABLE) {
              kputs("String table");
            } else if (section_header.type ==
                           STALLION_ELF_SECTION_RELOCATION_NO_ADDEND ||
                       section_header.type ==
                           STALLION_ELF_SECTION_RELOCATION_WITH_ADDEND) {

              kputs("Reloc");
            }
          }
        }
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
