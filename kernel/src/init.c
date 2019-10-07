#include <stallion.h>

void stallion_init(stallion_t *os) {
  // TODO: Does the boot_info struct need initialization info?
  os->boot_info.ram_start = 0;
  stallion_scheduler_init(&os->scheduler);
}

void stallion_scheduler_init(stallion_scheduler_t *scheduler) {
  scheduler->max_pid = 0;
  scheduler->processes = NULL;
}