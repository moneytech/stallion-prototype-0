#ifndef STALLION_PROCESS_H
#define STALLION_PROCESS_H
#include <stdint.h>

typedef struct _stallion_page_table stallion_page_table_t;

typedef struct _stallion_process {
  struct _stallion_process *prev, *next;
  uint64_t pid;
  uint16_t stallion_page_table_index;
  void* program_counter;
} stallion_process_t;

#endif
