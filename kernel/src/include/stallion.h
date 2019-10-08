#ifndef STALLION_STALLION_H
#define STALLION_STALLION_H
#define _HAVE_SIZE_T
#include "blob.h"
#include "fd.h"
#include "interrupt.h"
#include "multiboot2.h"
#include "stallion_elf.h"
#include "stallion_process.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "liballoc.h"

typedef struct {
  multiboot_uint64_t ram_start;
} stallion_boot_info_t;

typedef struct _stallion {
  stallion_boot_info_t boot_info;
  stallion_scheduler_t scheduler;
} stallion_t;

extern uint32_t startkernel;
extern uint32_t endkernel;
extern stallion_t *global_os; // TODO: Any alternative to a global?

void stallion_init(stallion_t *os);
void stallion_scheduler_run(stallion_scheduler_t *scheduler);
void stallion_return_from_ring3();

int32_t stallion_interrupt_handler(stallion_interrupt_t *ctx);
int32_t stallion_handle_general_protection_fault(stallion_interrupt_t *ctx);
void stallion_handle_page_fault(stallion_interrupt_t *ctx);
void *stallion_get_page_fault_pointer();
void stallion_early_init(stallion_t *os, unsigned long magic, void *addr);

/** Convert an integer to a string. */
char *kitoa(int value, char *str, int base);

/** Clears the screen.*/
void kcls();

/** Writes a single character to the screen. */
void kputc(char ch);

/** Writes a string to the screen. */
void kwrites(const char *text);

/** Writes a string to the screen, followed by a newline. */
void kputs(const char *text);

/** Writes an integer to the screen as base-10. */
void kwritei(int val);

/** Writes an integer to the screen. */
void kwritei_r(int val, int base);

/** Writes an integer to the screen as base-10. */
void kputi(int val);

/** Writes an integer to the screen. */
void kputi_r(int val, int base);

void kputptr(const char *label, void *ptr);

void kmemset(void *ptr, uint8_t value, size_t size);

void kmemcpy(void *dst, void *src, unsigned long size);

void kmemdump(void *addr, unsigned long size);

/**
 * Finds the length of a string that is KNOWN to be <= 65535 characters long.
 *  Should work in almost every case, honestly.
 */
uint16_t kshortstrlen(const char *text);

void stallion_page_get_indices(void *addr, uint32_t *pde_index,
                               uint32_t *pte_index);

uint32_t stallion_page_get_directory_size();

uint32_t stallion_page_get_table_size();

uint32_t stallion_page_get_page_size();

uint32_t stallion_page_get_flag_kernel();

uint32_t stallion_page_get_flag_user();

uint32_t stallion_page_get_flag_readwrite();

bool stallion_page_map(void *phys, void *virt, uint32_t flags);

size_t stallion_page_map_region(void *phys, void *virt, size_t size,
                                uint32_t flags);

void stallion_page_mark_readonly(void *phys, void *virt, size_t size);

bool stallion_page_unmap(void *virt);

size_t stallion_page_unmap_region(void *virt, size_t size);

static inline void hang() {
  kputs("HANGING");
  while (true)
    continue;
}

#endif
