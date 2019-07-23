message(STATUS "Compiling Stallion for i686.")
add_library(stallion_arch OBJECT
  boot.S
  gdt.c idt.c kernel.c paging.c
  interrupt.c serial_print.c
)
