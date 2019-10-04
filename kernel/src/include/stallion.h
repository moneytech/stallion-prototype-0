#ifndef STALLION_STALLION_H
#define STALLION_STALLION_H
#define _HAVE_SIZE_T
#include "multiboot2.h"
#include "process.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "liballoc.h"

#define STALLION_SYSCALL_EXIT 1
#define STALLION_SYSCALL_READ 2
#define STALLION_SYSCALL_WRITE 3
#define STALLION_SYSCALL_BLOCK 4

typedef struct {
  multiboot_uint64_t ram_start;
} stallion_boot_info_t;

typedef struct {
  stallion_boot_info_t boot_info;
} stallion_t;

typedef struct {
  uint32_t cr2;
  uint32_t gs;
  uint32_t fs;
  uint32_t ds;
  uint32_t es;
  uint32_t edi;
  uint32_t esi;
  uint32_t ebp;
  uint32_t ebx;
  uint32_t edx;
  uint32_t ecx;
  uint32_t eax;
  uint32_t number; // The interrupt number, i.e. 0x80
  uint32_t error_code;
  uint32_t cs;
  uint32_t eflags;
  uint32_t esp;
  uint32_t ss;
} __attribute__((packed)) stallion_interrupt_t;

extern uint32_t startkernel;
extern uint32_t endkernel;

void stallion_interrupt_handler(stallion_interrupt_t *ctx);
void stallion_handle_general_protection_fault(stallion_interrupt_t *ctx);
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

static inline void hang() {
  kputs("HANGING");
  while (true)
    continue;
}

#endif
