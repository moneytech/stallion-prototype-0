#ifndef STALLION_STALLION_H
#define STALLION_STALLION_H
#define _HAVE_SIZE_T
#include "process.h"
#include <multiboot2.h>
#include <stddef.h>
#include <stdint.h>

#include <liballoc.h>

#define COM1 0x3F8

typedef struct {
  multiboot_uint64_t ram_start;
} stallion_boot_info_t;

void init_gdt();
void init_idt();
void init_paging(stallion_boot_info_t *boot_info);

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
void kputi(int val);

/** Writes an integer to the screen. */
void kputi_r(int val, int base);

/**
 * Finds the length of a string that is KNOWN to be <= 65535 characters long.
 *  Should work in almost every case, honestly.
 */
uint16_t kshortstrlen(const char *text);

void *kmalloc(size_t size);
void kfree(void *ptr);

/** Sends a value through a serial port. */
static inline void outb(uint16_t port, uint8_t val) {
  asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

/** Receives a value from a serial port. */
static inline uint8_t inb(uint16_t port) {
  uint8_t ret;
  asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
  return ret;
}

static inline void interrupt(uint8_t no) { asm volatile("int %0" ::"Nd"(no)); }

#endif
