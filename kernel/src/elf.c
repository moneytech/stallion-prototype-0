#include <stallion_elf.h>

bool stallion_elf_read_header(void *data, size_t size,
                              stallion_elf_header_t **header) {
  if (data == NULL)
    return false;

  if (size < sizeof(stallion_elf_header_t))
    return false;

  stallion_elf_header_t *hdr = data;
  if (hdr->magic[0] != 0x7f)
    return false;
  if (hdr->magic[1] != 'E')
    return false;
  if (hdr->magic[2] != 'L')
    return false;
  if (hdr->magic[3] != 'F')
    return false;

  *header = hdr;
  return true;
}

bool stallion_elf_check_supported(stallion_elf_header_t *header,
                                  const char **error_message) {

  // TODO: 64-bit
  if (header->bitness != STALLION_ELF_BITNESS_32) {
    *error_message = "Only 32-bit ELF modules are supported.";
    return false;
  }

  if (header->endianness != STALLION_ELF_ENDIANNESS_LITTLE) {
    *error_message = "Only little-endian ELF modules are supported.";
    return false;
  }

  if (header->isa != STALLION_ELF_ISA_X86) {
    *error_message = "Only x86 ELF modules are supported.";
    return false;
  }

  if (header->elf_version != 1) {
    *error_message = "Only ELF v1 modules are supported.";
    return false;
  }

  if (header->attrs != STALLION_ELF_ATTR_EXECUTABLE &&
      header->attrs != STALLION_ELF_ATTR_RELOCATABLE) {
    *error_message = "Only relocatable/executable ELF modules are supported.";
    return false;
  }

  return true;
}

stallion_elf_section_header_t *
stallion_elf_get_section_header_array(stallion_elf_header_t *header) {
  return (stallion_elf_section_header_t *)(((uint32_t)header) +
                                           header->section_header_addr);
}

const char *
stallion_get_section_name(stallion_elf_header_t *header,
                          stallion_elf_section_header_t *section_header) {
  if (section_header->name_addr == 0)
    return "<unnamed>";
  else if (header->section_name_index == 0)
    return "<unnamed>";

  stallion_elf_section_header_t str_header =
      stallion_elf_get_section_header_array(header)[header->section_name_index];

  char **str_table = (char *)((uint32_t)&str_header + str_header.offset);
  return (const char *)(str_table[section_header->name_addr]);
}