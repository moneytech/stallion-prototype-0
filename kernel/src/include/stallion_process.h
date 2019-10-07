#ifndef STALLION_PROCESS_H
#define STALLION_PROCESS_H
#include "interrupt.h"
#include "stallion_elf.h"

typedef struct _stallion_process {
  size_t id;
  struct _stallion_process *prev, *next;
  bool started;
  stallion_elf_binary_t *binary;
  stallion_interrupt_t last_state;
} stallion_process_t;

typedef struct {
  size_t max_pid;
  stallion_process_t *processes;
  stallion_process_t *current_process;
} stallion_scheduler_t;

void stallion_enter_ring3(void *entry_point);

void stallion_scheduler_init(stallion_scheduler_t *scheduler);

stallion_process_t *
stallion_scheduler_enqueue_binary(stallion_scheduler_t *scheduler,
                                  stallion_elf_binary_t *binary);

void stallion_kill_current_process(stallion_scheduler_t *scheduler);

#endif