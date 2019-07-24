cmake_minimum_required(VERSION 3.0)
project(stallion)
include(ExternalProject)
add_subdirectory(toolchain)
ExternalProject_Add(kernel SOURCE_DIR 
  ${CMAKE_CURRENT_LIST_DIR}/kernel
  CONFIGURE_COMMAND 
    cd <BINARY_DIR> &&
    ${CMAKE_COMMAND} -DCMAKE_TOOLCHAIN_FILE=<SOURCE_DIR>/cmake/i686_elf_gcc.cmake <SOURCE_DIR>
  INSTALL_COMMAND "")
ExternalProject_Get_Property(kernel binary_dir)
set(STALLION_ISO ${binary_dir}/stallion.iso)

# Run the kernel...
set(QEMU qemu-system-i386)
add_custom_target(run_qemu
  COMMAND ${QEMU} -cdrom ${STALLION_ISO} -serial file:/dev/stdout
  DEPENDS kernel)
