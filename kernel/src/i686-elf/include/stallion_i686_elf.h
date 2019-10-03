#ifndef STALLION_STALLION_I686_H
#define STALLION_STALLION_I686_H
#include <stallion.h>
#include <stddef.h>
#include <stdint.h>

#define COM1 0x3F8
#define PAGE_DIRECTORY_SIZE 1024
#define PAGE_TABLE_SIZE PAGE_DIRECTORY_SIZE
#define PAGE_SIZE 4096
#define PAGE_MASK_EMPTY 0xfffff000
#define PAGE_MASK_PRESENT 0x1
#define PAGE_MASK_RING0 0x4

typedef struct _stallion_page_table {
  uint32_t pages[PAGE_TABLE_SIZE] __attribute__((aligned(4096)));
} __attribute__((packed)) stallion_page_table_t;

void stallion_init_gdt(stallion_t *os);
void stallion_init_idt(stallion_t *os);
void stallion_init_paging(stallion_t *os);
extern void stallion_gdt_enable(uint32_t gdtr);

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
