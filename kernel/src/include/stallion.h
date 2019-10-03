#ifndef STALLION_STALLION_H
#define STALLION_STALLION_H
#define _HAVE_SIZE_T
#include "process.h"
#include "multiboot2.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "liballoc.h"

typedef struct {
  multiboot_uint64_t ram_start;
} stallion_boot_info_t;

typedef struct {
  stallion_boot_info_t boot_info;
} stallion_t;

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

void kputptr(const char* label, void* ptr);

void kmemset(void *ptr, uint8_t value, size_t size);

void kmemcpy(void *dst, void *src, unsigned long size);

/**
 * Finds the length of a string that is KNOWN to be <= 65535 characters long.
 *  Should work in almost every case, honestly.
 */
uint16_t kshortstrlen(const char *text);

void *kmalloc(size_t size);
void kfree(void *ptr);

uint32_t stallion_page_get_indices(void* addr);

uint32_t stallion_page_get_directory_size();

uint32_t stallion_page_get_table_size();

uint32_t stallion_page_get_page_size();

uint32_t stallion_page_get_flag_kernel();

uint32_t stallion_page_get_flag_user();

uint32_t stallion_page_get_flag_readwrite();

bool stallion_page_map(void* phys, void* virt, uint32_t flags);

size_t stallion_page_map_region(void* phys, void* virt, size_t size, uint32_t flags);

#endif
