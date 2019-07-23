#include <multiboot2.h>
#include <stallion.h>

void kernel_main(unsigned long magic, void *addr) {
  kputs("Entered Stallion kernel.");
  kwrites("magic=0x");
  kputi_r(magic, 16);
  kputc('\n');

  if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
    kputs("Invalid magic given; not Multiboot2. Aborting.");
    return;
  }

  kputs("Correct magic - Multiboot2 detected.");

  // Read all the tags.
  uint32_t mbi_size = *(uint32_t *)addr;
  kwrites("Multiboot2 header tag size: ");
  kputi(mbi_size);
  kputc('\n');

  struct multiboot_tag *tag = addr + 8; // Skip size, AND reserved u32 field.
  while (tag->type != MULTIBOOT_TAG_TYPE_END) {
    switch (tag->type) {
    case MULTIBOOT_TAG_TYPE_CMDLINE: {
      struct multiboot_tag_string *cmdline = (struct multiboot_tag_string *)tag;
      kwrites("Boot command line: ");
      kputs(cmdline->string);
    } break;
    case MULTIBOOT_TAG_TYPE_MMAP: {
      struct multiboot_tag_mmap *mmap_tag = (struct multiboot_tag_mmap *)tag;
      multiboot_memory_map_t *mmap = mmap_tag->entries;
      while (mmap < (multiboot_memory_map_t *)(tag + tag->size)) {
        if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE) {
          // Print info.
          kwrites("Found available RAM at 0x");
          kputi_r(mmap->addr, 16);
          // TODO: Why did the GRUB example include these shifts, etc.?
          // kputi_r(mmap->addr >> 3, 16);
          // kputi_r(mmap->addr & 0xffffffff, 16);
          kwrites(" of size ");
          kputi(mmap->len);
          kwrites("; type=");
          kputi(mmap->type);
          kputc('\n');
        }
        // Jump to the next.
        mmap += mmap_tag->entry_size;
      }
    } break;
    }
    // Jump to the next one.
    tag = (struct multiboot_tag *)((multiboot_uint8_t *)tag +
                                   ((tag->size + 7) & ~7));
  }

  kputs("Done reading tags.");
}

