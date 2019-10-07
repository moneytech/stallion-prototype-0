#ifndef STALLION_INTERRUPT_H
#define STALLION_INTERRUPT_H
#include <stdint.h>

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

#endif