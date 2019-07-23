message(STATUS "Compiling Stallion for i686.")
add_library(stallion_arch OBJECT
  boot.S
  kernel.c
)
