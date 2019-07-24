include_directories(include)
include_directories(newlib/newlib/libc/include)
include_directories(newlib/newlib/libm/include)
add_library(stallion_userland OBJECT syscalls.c)
