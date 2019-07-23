#ifndef STALLION_PROCESS_H
#define STALLION_PROCESS_H
#include <stdint.h>

typedef struct _stallion_process {
  struct _stallion_process *prev, *next;
  uint64_t pid;
  uint16_t page_table_index;
} stallion_process_t;

#endif
