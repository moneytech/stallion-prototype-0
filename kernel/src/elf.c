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