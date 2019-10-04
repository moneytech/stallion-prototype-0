#ifndef STALLION_ELF_H
#define STALLION_ELF_H

#include "stallion.h"

#define STALLION_ELF_BITNESS_32 1
#define STALLION_ELF_BITNESS_64 2
#define STALLION_ELF_ENDIANNESS_LITTLE 1
#define STALLION_ELF_ENDIANNESS_BIG 2
#define STALLION_ELF_ABI_SYSTEM_V 0
#define STALLION_ELF_ATTR_RELOCATABLE 1
#define STALLION_ELF_ATTR_EXECUTABLE 2
#define STALLION_ELF_ATTR_SHARED 3
#define STALLION_ELF_ATTR_CORE 4
#define STALLION_ELF_ISA_X86 3
#define STALLION_ELF_ISA_X64 0x32

typedef struct {
  uint8_t magic[4];
  uint8_t bitness;
  uint8_t endianness;
  uint8_t header_version;
  uint8_t abi;
  uint64_t unused;
  uint16_t attrs;
  uint16_t isa;
  uint32_t elf_version;
  uint32_t entry_point;
  uint32_t program_header_addr;
  uint32_t section_header_addr;
  uint32_t flags;
  uint16_t header_size;
  uint16_t program_header_entry_size;
  uint16_t program_header_entry_count;
  uint16_t section_header_entry_size;
  uint16_t section_header_entry_count;
  uint16_t section_name_index;
} stallion_elf_header32_t;

typedef stallion_elf_header32_t stallion_elf_header_t;

bool stallion_elf_read_header(void *data, size_t size,
                              stallion_elf_header_t **header);

#endif