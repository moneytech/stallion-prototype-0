#include <multiboot2.h>
#include <stallion.h>
#define PAGE_DIRECTORY_SIZE 1024
#define PAGE_TABLE_SIZE PAGE_DIRECTORY_SIZE
#define PAGE_SIZE 4096
#define PAGE_MASK_EMPTY 0xfffff000
#define PAGE_MASK_PRESENT 0x1
#define PAGE_MASK_RING0 0x4

extern uint32_t startkernel;
extern uint32_t endkernel;

struct _stallion_page_table {
  uint32_t pages[PAGE_TABLE_SIZE] __attribute__((aligned(4096)));
} __attribute__((packed));

uint32_t page_directory[PAGE_DIRECTORY_SIZE] __attribute__((aligned(4096)));
stallion_page_table_t page_tables[PAGE_DIRECTORY_SIZE];

void init_paging(stallion_boot_info_t *boot_info) {
  // First, set up our page directory. Every entry should point to
  // a page table.
  for (uint16_t i = 0; i < PAGE_DIRECTORY_SIZE; i++) {
    stallion_page_table_t *table = &page_tables[i];
    page_directory[i] = (uint32_t)table & PAGE_MASK_EMPTY;

    // While we loop, also initialize each page table.
    for (uint16_t j = 0; j < PAGE_TABLE_SIZE; j++) {
      // Each page points to a 4KB offset from ram_start.
      multiboot_uint64_t addr = boot_info->ram_start + (PAGE_SIZE * j);
      table->pages[0] = addr & PAGE_MASK_EMPTY;
    }
  }

  // Next, identity map the kernel.
  // Simply put: We simply set (present) on pages in the first
  // page table, until we have passed the end of the kernel.
  uint32_t offset = 0;
  uint16_t id_mapped_pages = 0;
  page_directory[0] |= PAGE_MASK_PRESENT;
  while (offset < endkernel) {
    page_tables[0].pages[id_mapped_pages] =
        (id_mapped_pages * PAGE_SIZE) | PAGE_MASK_PRESENT;
    id_mapped_pages++;
    offset += PAGE_SIZE;
  }

	// Next, load the page directory, and enable paging.
}
