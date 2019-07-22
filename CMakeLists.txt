cmake_minimum_required(VERSION 3.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
project(stallion C CXX ASM)
include_directories(include)
add_subdirectory(src)
message(STATUS "Loaded CMake Toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
message(STATUS "C Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "Assembler: ${CMAKE_ASM_COMPILER}")

file(GLOB_RECURSE iso_contents "iso_contents/**/*")
add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/stallion.iso"
  MAIN_DEPENDENCY stallion_bin
  SOURCES ${iso_contents}
  BYPRODUCTS "${CMAKE_CURRENT_BINARY_DIR}/stallion.iso"
  COMMAND grub-file --is-x86-multiboot $<TARGET_FILE:stallion_bin>
  COMMAND cp -rp "${CMAKE_CURRENT_LIST_DIR}/iso_contents" "${CMAKE_CURRENT_BINARY_DIR}/isodir"
  COMMAND cp $<TARGET_FILE:stallion_bin> "${CMAKE_CURRENT_BINARY_DIR}/isodir/boot"
  COMMAND grub-mkrescue -o "${CMAKE_CURRENT_BINARY_DIR}/stallion.iso" "${CMAKE_CURRENT_BINARY_DIR}/isodir"
)
add_custom_target(stallion_iso ALL DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/stallion.iso")
