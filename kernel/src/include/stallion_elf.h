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
#define STALLION_ELF_SECTION_NULL 0
#define STALLION_ELF_SECTION_PROGRAM_INFO 1
#define STALLION_ELF_SECTION_SYMBOL_TABLE 2
#define STALLION_ELF_SECTION_STRING_TABLE 3
#define STALLION_ELF_SECTION_RELOCATION_WITH_ADDEND 4
#define STALLION_ELF_SECTION_NOT_PRESENT 5
#define STALLION_ELF_SECTION_RELOCATION_NO_ADDEND 6
#define STALLION_ELF_TABLE_WRITABLE 1
#define STALLION_ELF_TABLE_MEMORY 2

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

typedef struct {
  uint32_t name_addr;
  uint32_t type;
  uint32_t flags;
  uint32_t addr;
  uint32_t offset;
  uint32_t size;
  uint32_t link;
  uint32_t info;
  uint32_t addr_align;
  uint32_t entry_size;
} stallion_elf_section_header_32_t;

typedef stallion_elf_header32_t stallion_elf_header_t;
typedef stallion_elf_section_header_32_t stallion_elf_section_header_t;

typedef struct _stallion_elf_symbol_table {
  struct _stallion_elf_symbol_table *next;
} stallion_elf_symbol_table_t;

typedef struct _stallion_elf_string_table {
  struct _stallion_elf_string_table *next;
} stallion_elf_string_table_t;

typedef struct {
  stallion_elf_symbol_table_t *symbol_table;
  stallion_elf_string_table_t *string_table;
} stallion_elf_binary_t;

bool stallion_elf_read_header(void *data, size_t size,
                              stallion_elf_header_t **header);

bool stallion_elf_check_supported(stallion_elf_header_t *header,
                                  const char **error_message);

stallion_elf_section_header_t *
stallion_elf_get_section_header_array(stallion_elf_header_t *header);

stallion_elf_binary_t *stallion_elf_binary_create();

bool stallion_elf_read_tables(stallion_elf_header_t *header,
                              stallion_elf_binary_t *binary,
                              const char **error_message);

bool stallion_elf_read_binary(void *data, size_t size,
                              stallion_elf_binary_t *binary,
                              const char **error_message);

// const char *
// stallion_get_section_name(stallion_elf_header_t *header,
//                           stallion_elf_section_header_t *section_header);

#endif