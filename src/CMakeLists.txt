cmake_policy(SET CMP0026 OLD)
set(TARGET i686-elf)
set(CMAKE_AR ${TARGET}-ar)
set(CMAKE_C_COMPILER ${TARGET}-gcc)
set(CMAKE_RANLIB  ${TARGET}-ranlib)
set(CMAKE_C_FLAGS "--std=gnu99 -ffreestanding -O2 -Wall -Wextra")
message(STATUS ${CMAKE_C_COMPILER})


add_subdirectory(arch)
add_subdirectory(kernel)

get_property(arch_location TARGET arch PROPERTY LOCATION)
get_property(kernel_location TARGET kernel PROPERTY LOCATION)

set(bin_location ${CMAKE_CURRENT_BINARY_DIR}/stallion.bin)
set(iso_location ${CMAKE_CURRENT_BINARY_DIR}/stallion.iso)

add_custom_target(bin
        ALL
        COMMAND ${CMAKE_C_COMPILER}
            -T ${CMAKE_CURRENT_LIST_DIR}/linker.ld
            -o ${bin_location} -ffreestanding -O2 -nostdlib
            ${arch_location} ${kernel_location} -lgcc
        DEPENDS arch
        DEPENDS kernel)

add_custom_target(verify
        COMMAND echo Verifying ${bin_location}...
        COMMAND grub-file --is-x86-multiboot ${bin_location}
        DEPENDS bin)

add_custom_target(qemu
        COMMAND qemu-system-i386 -kernel ${bin_location}
        DEPENDS bin
        DEPENDS verify)