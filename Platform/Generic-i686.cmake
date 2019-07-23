message(STATUS "Building for `Generic`....")
# enable_language(ASM)
# set(CMAKE_ASM_COMPILER i686-elf-as)
set(QEMU qemu-system-i386)

# set(CMAKE_ASM_COMPILE_OBJECT "<CMAKE_C_COMPILER> <INCLUDES> \
#     <FLAGS> -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -nostdlib -o <OBJECT> <SOURCE>")
# if(("${CMAKE_LINKER}" EQUAL "") OR ("${CMAKE_LINKER}" MATCHES "NOTFOUND"))
#   set(CMAKE_LINKER i686-elf-ld)
# endif()