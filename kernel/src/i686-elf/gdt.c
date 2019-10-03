#include <multiboot2.h>
#include <stallion.h>
#include <stallion_i686_elf.h>

typedef struct {
  uint16_t size;
  uint32_t offset;
} __attribute__((packed)) gdt_descriptor_t;

typedef struct {
  uint16_t limit_0_15;
  uint16_t base_0_15;
  uint8_t base_16_23;
  uint8_t access;
  uint8_t limit_16_19_and_flags;
  uint8_t base_24_31;
} __attribute__((packed)) gdt_entry_t;

#define LIMIT_4GB 0xffffffff

gdt_descriptor_t gdt_descriptor;
gdt_entry_t gdt[5];

/** Fills a GDT entry. */
static inline void gdt_describe(uint32_t i, uint32_t base, uint32_t limit,
                                uint8_t access, uint8_t flags) {
  // Set the base values
  gdt_entry_t *entry = &gdt[i];
  entry->base_0_15 = base & 0xffff;
  entry->base_16_23 = (base >> 16) & 0xff;
  entry->base_24_31 = (base >> 24) & 0xff;

  // Set the access
  entry->access = access;

  // Set the limit
  entry->limit_0_15 = limit & 0xffff;

  // Set the flags
  entry->limit_16_19_and_flags = (limit >> 16) & 0x0f;
  entry->limit_16_19_and_flags |= (flags & 0xf0);
}

// We don't use segmentation, so we provided a bare-minimum GDT.
void stallion_init_gdt(stallion_t *os) {
  gdt_describe(0, 0, 0, 0, 0);
  gdt_describe(1, 0, LIMIT_4GB, 0x9a, 0xcf);
  gdt_describe(2, 0, LIMIT_4GB, 0x92, 0xcf);
  gdt_describe(3, 0, LIMIT_4GB, 0xfa, 0xcf);
  gdt_describe(4, 0, LIMIT_4GB, 0xf2, 0xcf);

  // Describe the actual GDT.
  gdt_descriptor.offset = (uint32_t)&gdt;
  gdt_descriptor.size = (sizeof(gdt_entry_t) * 5) - 1;
  // gdt_descriptor.size = sizeof(gdt) - 1;

  // Load it!
  // asm("lgdt %0" ::"m"(gdt_descriptor));
  stallion_gdt_enable((uint32_t)&gdt_descriptor);
}
