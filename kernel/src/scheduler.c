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
  return p;
}