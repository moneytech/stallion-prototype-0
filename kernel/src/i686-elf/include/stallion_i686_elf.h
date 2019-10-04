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

typedef struct {
  uint32_t prev_tss;
  uint32_t esp0;
  uint32_t ss0;
  uint32_t esp1;
  uint32_t ss1;
  uint32_t esp2;
  uint32_t ss2;
  uint32_t cr3;
  uint32_t eip;
  uint32_t eflags;
  uint32_t eax;
  uint32_t ecx;
  uint32_t edx;
  uint32_t ebx;
  uint32_t esp;
  uint32_t ebp;
  uint32_t esi;
  uint32_t edi;
  uint32_t es;
  uint32_t cs;
  uint32_t ss;
  uint32_t ds;
  uint32_t fs;
  uint32_t gs;
  uint32_t ldt;
  uint16_t trap;
  uint16_t iomap_base;
} __attribute__((packed)) stallion_tss_entry_t;

extern stallion_tss_entry_t stallion_tss;
extern void stallion_tss_flush(uint32_t ptr);

void stallion_init_gdt(stallion_t *os);
void stallion_init_idt(stallion_t *os);
void stallion_init_paging(stallion_t *os);
extern void stallion_gdt_enable(uint32_t gdtr);
extern void stallion_paging_enable(uint32_t pde);

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
