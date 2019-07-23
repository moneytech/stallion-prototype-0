#include <multiboot2.h>
#include <stallion.h>

void kernel_main(unsigned long magic, void *addr) {
  stallion_boot_info_t boot_info;
  boot_info.ram_start = 0x0;
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
          // TODO: Handle multiple available RAM.
          boot_info.ram_start = mmap->addr;
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
    case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME: {
      kwrites("Booted by: ");
      kputs(((struct multiboot_tag_string *)tag)->string);
    } break;

    case MULTIBOOT_TAG_TYPE_MODULE: {
      struct multiboot_tag_module *module = (struct multiboot_tag_module *)tag;
      kwrites("Found module (0x");
      kputi_r(module->mod_start, 16);
      kwrites("-0x");
      kputi_r(module->mod_end, 16);
      kwrites("); cmdline=");
      kputs(module->cmdline);
    } break;
    case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO: {
      struct multiboot_tag_basic_meminfo *info =
          (struct multiboot_tag_basic_meminfo *)tag;
      kwrites("Lower memory: 0x");
      kputi_r(info->mem_lower, 16);
      kwrites("\nUpper memory: 0x");
      kputi_r(info->mem_upper, 16);
      kputc('\n');
    } break;
    case MULTIBOOT_TAG_TYPE_BOOTDEV: {
      struct multiboot_tag_bootdev *bootdev =
          (struct multiboot_tag_bootdev *)tag;
      kwrites("Root device drive number=0x");
      kputi_r(bootdev->biosdev, 16);
      kwrites("\nPartition number=0x");
      kputi_r(bootdev->slice, 16);
      kwrites("\nSub-partition number=0x");
      kputi_r(bootdev->part, 16);
      kputc('\n');
    } break;
    }
    // Jump to the next one.
    tag = (struct multiboot_tag *)((multiboot_uint8_t *)tag +
                                   ((tag->size + 7) & ~7));
  }

  kputs("Done reading tags.");

  // We are done reading tags.
  // Next, we set up our GDT, and then IDT.
  init_gdt();
  kputs("GDT initialized.");
  init_idt();
  kputs("IDT initialized.");

  //
  // After our interrupts are in place, and our kernel is ID-mapped,
  // We can start our scheduler, so we can begin running processes.
  init_paging(&boot_info);
  kputs("Paging set up.");

  // Once our scheduler is up, our system is (theoretically) ready to
  // start running userspace programs!
  //
  // Before we can run programs, we need to find them, and load the
  // physical system. Search for available disks, and mount...
  //
  // Now that we have mounted disks
}

