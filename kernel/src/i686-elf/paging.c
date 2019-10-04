#include <multiboot2.h>
#include <stallion.h>
#include <stallion_i686_elf.h>

uint32_t page_directory[PAGE_DIRECTORY_SIZE] __attribute__((aligned(4096)));
stallion_page_table_t page_tables[PAGE_DIRECTORY_SIZE];

void stallion_init_paging(stallion_t *os) {
  // Initialize the page directory to zeroes.
  kmemset(page_directory, 0, sizeof(page_directory));

  // ID-map the kernel.
  stallion_page_map_region(
      // &startkernel, &startkernel, &endkernel - &startkernel,
      (void *)0, (void *)0, (uint32_t)&endkernel,
      stallion_page_get_flag_readwrite() || stallion_page_get_flag_kernel());
  stallion_paging_enable((uint32_t)&page_directory);
}

void stallion_page_get_indices(void *addr, uint32_t *pde_index,
                               uint32_t *pte_index) {
  if (pde_index != NULL)
    *pde_index = (uint32_t)addr >> 22;
  if (pte_index != NULL)
    *pte_index = (uint32_t)addr >> 12 & 0x03FF;
}

uint32_t stallion_page_get_directory_size() { return PAGE_DIRECTORY_SIZE; }

uint32_t stallion_page_get_table_size() { return PAGE_TABLE_SIZE; }

uint32_t stallion_page_get_page_size() { return PAGE_SIZE; }

uint32_t stallion_page_get_flag_kernel() { return 0x0; }

uint32_t stallion_page_get_flag_user() {
  return 0x4; // 0x100
}

uint32_t stallion_page_get_flag_readwrite() {
  return 0x2; // 0x10
}

bool stallion_page_map(void *phys, void *virt, uint32_t flags) {
  uint32_t pde_index, pte_index;
  stallion_page_get_indices(virt, &pde_index, &pte_index);
  if (pde_index >= PAGE_DIRECTORY_SIZE)
    return false;
  if (pte_index >= PAGE_TABLE_SIZE)
    return false;

  // Determine if the directory is already present, by OR-ing against 0x1.
  if ((page_directory[pde_index] & 0x1) != 0x1) {
    // Create a page directory, pointing to the page table.
    page_directory[pde_index] =
        ((uint32_t)&page_tables[pde_index]) | (flags & 0xFFF) | 0x01;
    for (int i = 0; i < stallion_page_get_table_size(); i++) {
      page_tables[pde_index].pages[i] = 0x0;
    }
  }

  // Check if the page mapping is present. If so, return false.
  uint32_t *pt = &page_tables[pde_index].pages[pte_index];
  if ((*pt & 0x1) == 0x1)
    return false;

  // Otherwise, set the value, and return true.
  *pt = ((uint32_t)phys) | (flags & 0xFFF) | 0x01;
  return true;
}

size_t stallion_page_map_region(void *phys, void *virt, size_t size,
                                uint32_t flags) {
  size_t count = 0;
  void *orig_virt = virt;
  while (count < size) {
    if (stallion_page_map(phys, virt, flags)) {
      phys += PAGE_SIZE;
      virt += PAGE_SIZE;
      count += PAGE_SIZE;
    } else {
      break;
    }
  }
  return count;
}

bool stallion_page_unmap(void *virt) {
  uint32_t pde_index, pte_index;
  stallion_page_get_indices(virt, &pde_index, &pte_index);
  if (pde_index >= PAGE_DIRECTORY_SIZE)
    return true;
  if (pte_index >= PAGE_TABLE_SIZE)
    return true;

  // Determine if the directory is already present, by OR-ing against 0x1.
  if ((page_directory[pde_index] & 0x1) != 0x1) {
    return true;
  }

  // Check if the page mapping is present. If so, return false.
  uint32_t *pt = &page_tables[pde_index].pages[pte_index];

  // Otherwise, set the value, and return true.
  *pt = 0x0;
  return true;
}

size_t stallion_page_unmap_region(void *virt, size_t size) {
  size_t count = 0;
  void *orig_virt = virt;
  while (count < size) {
    if (stallion_page_unmap(virt)) {
      virt += PAGE_SIZE;
      count += PAGE_SIZE;
    } else {
      break;
    }
  }
  return count;
}

int liballoc_lock() { return 0; }

int liballoc_unlock() { return 0; }

void *liballoc_alloc(int page_count) {
  // TODO: There's opportunity to optimize this, i.e. by using a stack.

  // Skip the ID-mapped region.
  void *base_ptr = &endkernel;

  while (base_ptr < (void *)0xffffffff) {
    int avail = 0;

    for (int i = 0; i < page_count; i++) {
      void *ptr = base_ptr + (i * PAGE_SIZE);
      uint32_t pde_index, pte_index;
      stallion_page_get_indices(ptr, &pde_index, &pte_index);
      // If the PDE is not present, then the PTE is obviously unmapped.
      if ((page_directory[pde_index] & 0x1) != 0x1) {
        avail++;
        continue;
      }

      // Otherwise, test the PTE.
      if ((page_tables[pde_index].pages[pte_index] & 0x1) != 0x1) {
        avail++;
        continue;
      }

      break;
    }

    // Map all of the found pages.
    if (avail == page_count) {
      for (int i = 0; i < page_count; i++) {
        void *ptr = base_ptr + (i * PAGE_SIZE);
        stallion_page_map(ptr, ptr,
                          stallion_page_get_flag_kernel() |
                              stallion_page_get_flag_readwrite());
      }
      return base_ptr;
    } else {
      base_ptr += (avail + 1) * PAGE_SIZE;
    }
  }

  return NULL;
}

int liballoc_free(void *base_ptr, int page_count) {
  // Unmap all of the pages.
  // TODO: Free page directory if it's unused.
  for (int i = 0; i < page_count; i++) {
    void *ptr = base_ptr + (i * PAGE_SIZE);
    if (!stallion_page_unmap(ptr))
      return 1;
  }
  return 0;
}
