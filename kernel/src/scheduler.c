#include <stallion.h>

stallion_process_t *
stallion_scheduler_enqueue_binary(stallion_scheduler_t *scheduler,
                                  stallion_elf_binary_t *binary) {
  stallion_process_t *p = kmalloc(sizeof(stallion_process_t));
  if (p == NULL)
    return p;

  p->binary = binary;
  p->id = scheduler->max_pid++;
  p->next = NULL;
  p->prev = scheduler->processes;
  p->started = false;
  if (scheduler->processes != NULL) {
    scheduler->processes->next = p;
  }
  if (scheduler->current_process == NULL) {
    scheduler->current_process = p;
  }
  return p;
}

void stallion_scheduler_run(stallion_scheduler_t *scheduler) {
  if (scheduler->current_process != NULL) {
    // TODO: Move ELF loading logic, and only load modules into memory
    // when they are about to run.
  }
}