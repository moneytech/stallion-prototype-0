message(STATUS "Compiling Stallion for i686.")
add_library(stallion_arch OBJECT
  boot.s
  kernel.c
)

set_property(SOURCE boot.s PROPERTY LANGUAGE C)