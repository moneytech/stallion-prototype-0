#include <stallion.h>

stallion_t *global_os;

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
  // TODO: Robust error handling.
  stallion_process_t *proc = scheduler->current_process;
  if (proc == NULL) {
    kputs("Kernel done");
    hang();
  } else {
    // Load the ELF binary into memory.
    if (!stallion_elf_load_into_memory(proc->binary)) {
      kputs("Failed to load ELF binary into memory.");
      return;
    }

    // Now that we've loaded the process into memory, just
    // execute it.
    // TODO: Resume processes using saved instruction pointer.
    void *entry_point = (void *)proc->binary->header->entry_point;
    proc->started = true;
    // typedef void (*Unsafe)();
    // Unsafe unsafe = entry_point;
    // unsafe();
    stallion_enter_ring3(entry_point);
  }
}

void stallion_kill_current_process(stallion_scheduler_t *scheduler) {
  stallion_process_t *proc = scheduler->current_process;
  if (proc != NULL) {
    stallion_process_t *old = proc;
    stallion_elf_unload_from_memory(proc->binary);
    if (scheduler->processes == old) {
      scheduler->processes = old->next;
    }
    if (old->next != NULL) {
      scheduler->current_process = old->next;
    } else {
      scheduler->current_process = scheduler->processes;
    }
    kfree(old);
  }
}

void stallion_return_from_ring3() {
  kputs("BACK FROM RING3");
  stallion_scheduler_run(&global_os->scheduler);
  // hang();
}