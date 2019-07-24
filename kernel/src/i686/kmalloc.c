#include <stallion.h>

// The kernel has 4GB of memory, at least on a 32-bit system.
// This memory has already been ID-paged, and we will handle it
// by dividing it into smaller pages.
//
// We manage a stack that keeps track of allocated blocks and their sizes.
// Assuming that most allocations are short-term, this will
// make `free` O(N) at worst, but usually less than that amount.
//
//

#define GIB4 1024 * 1024 * 4
#define KMALLOC_FRAME_SIZE (sizeof(int) * 4)
#define KMALLOC_FRAME_COUNT (GIB4 / KMALLOC_FRAME_SIZE)

int liballoc_lock() { return 0; }

int liballoc_unlock() { return 0; }

uint8_t frames[KMALLOC_FRAME_COUNT] = {0};

void *liballoc_alloc(int page_count) {
  for (int i = 0; i < KMALLOC_FRAME_COUNT; i++) {
    int avail = 0;
    for (int j = 0; j < page_count; j++) {
      if (frames[i + j] == 0)
        avail++;
    }
    if (avail == page_count) {
      for (int j = 0; j < page_count; j++) {
        frames[i + j] = 1;
      }
      return (void *)(i * KMALLOC_FRAME_SIZE);
    }
  }
  return NULL;
}

int liballoc_free(void *ptr, int page_count) {
  int i = (int)ptr;
  for (int j = 0; j < page_count; j++) {
    frames[i + j] = 0;
  }
  return 0;
}
