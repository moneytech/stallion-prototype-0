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

stallion_elf_program_header_t *
stallion_elf_get_program_header_array(stallion_elf_header_t *header) {

  return (stallion_elf_program_header_t *)(((uint32_t)header) +
                                           header->program_header_addr);
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

  char **str_table = (char **)((uint32_t)&str_header + str_header.offset);
  return (const char *)(str_table[section_header->name_addr]);
}

stallion_elf_binary_t *stallion_elf_binary_create() {
  stallion_elf_binary_t *out =
      (stallion_elf_binary_t *)kmalloc(sizeof(stallion_elf_binary_t));
  if (out != NULL) {
    out->next = NULL;
    out->header = NULL;
    out->string_table = NULL;
    out->symbol_table = NULL;
    out->executable_regions = NULL;
  }
  return out;
}

bool stallion_elf_read_tables(stallion_elf_header_t *header,
                              stallion_elf_binary_t *binary,
                              const char **error_message) {
  // TODO: Is reading section headers actually necessary?

  // Read the headers...
  stallion_elf_section_header_t *section_headers =
      stallion_elf_get_section_header_array(header);

  for (uint16_t i = 0; i < header->section_header_entry_count; i++) {
    stallion_elf_section_header_t section_header = section_headers[i];
    if (section_header.type == STALLION_ELF_SECTION_SYMBOL_TABLE) {
      // kputs("Symbol table");
    } else if (section_header.type == STALLION_ELF_SECTION_STRING_TABLE) {
      // kputs("String table");
    } else if (section_header.type ==
                   STALLION_ELF_SECTION_RELOCATION_NO_ADDEND ||
               section_header.type ==
                   STALLION_ELF_SECTION_RELOCATION_WITH_ADDEND) {
      // kputs("Reloc");
    }
  }

  return true;
}

bool stallion_elf_read_binary(void *data, size_t size,
                              stallion_elf_binary_t *binary,
                              const char **error_message) {

  stallion_elf_header_t *header;
  if (!stallion_elf_read_header(data, size, &header)) {
    *error_message = "Could not read ELF header.";
    return false;
  } else {
    binary->header = header;
    if (!stallion_elf_check_supported(header, error_message)) {
      return false;
    } else {
      if (!stallion_elf_read_tables(header, binary, error_message)) {
        return false;
      } else {
        // Load the program headers.
        stallion_elf_program_header_t *program_headers =
            stallion_elf_get_program_header_array(header);
        for (uint16_t i = 0; i < header->program_header_entry_count; i++) {
          stallion_elf_program_header_t ph = program_headers[i];
          if (ph.type == STALLION_ELF_PROGRAM_LOAD) {
            // load - clear p_memsz bytes at p_vaddr to 0, then copy p_filesz
            // bytes from p_offset to p_vaddr
            void *vaddr = (void *)ph.virtual_memory_offset;
            void *faddr = (void *)data + ph.offset_in_file;
            void *phys_addr = vaddr;
            // uint32_t flags = stallion_page_get_flag_user();
            uint32_t flags = stallion_page_get_flag_kernel();
            // Only make pages R/W if the section is writable.
            if (ph.flags == 0x2) {
              flags |= stallion_page_get_flag_readwrite();
            }
            stallion_page_map_region(vaddr, phys_addr, ph.size_in_memory, flags);
            kmemcpy(vaddr, faddr, ph.size_in_file);
            // Instead of filling the remaining space with zeroes, load it
            // with RET instructions.
            kmemset(vaddr + ph.size_in_file, 0, ph.size_in_memory - ph.size_in_file);
            kputs("LOADED");
            kputptr("vaddr", vaddr);
            kputptr("phys_addr", phys_addr);
            kputptr("faddr", faddr);
            kwrites("Copied size: 0x");
            kputi_r(ph.size_in_file, 16);
            kwrites("Memory size: 0x");
            kputi_r(ph.size_in_memory, 16);

            // TODO: Handle out-of-memory
            stallion_elf_executable_region_t *region =
                kmalloc(sizeof(stallion_elf_executable_region_t));
            region->header = ph;
            region->vaddr = vaddr;
            region->next = binary->executable_regions;
            binary->executable_regions = region;
          } else if (ph.type == STALLION_ELF_PROGRAM_DYNAMIC) {
            // TODO: Dynamic linking...
            // kputs("DYNAMIC");
          }
        }
        return true;
      }
    }
  }
}