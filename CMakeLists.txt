cmake_minimum_required(VERSION 3.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}")
list(APPEND CMAKE_C_SOURCE_FILE_EXTENSIONS "s")
project(stallion)
enable_language(ASM)
include(Platform/${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}.cmake)
include_directories(iso_contents/boot/include)
add_subdirectory(src)
add_subdirectory(toolchain)
message(STATUS "Loaded CMake Toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
message(STATUS "C Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "Assembler: ${CMAKE_ASM_COMPILER}")

file(GLOB_RECURSE iso_contents "iso_contents/**/*")
set(STALLION_ISO "${CMAKE_CURRENT_BINARY_DIR}/stallion.iso")
add_custom_command(OUTPUT ${STALLION_ISO} 
  MAIN_DEPENDENCY stallion_bin
  SOURCES ${iso_contents}
  BYPRODUCTS ${STALLION_ISO}
  COMMAND grub-file --is-x86-multiboot2 $<TARGET_FILE:stallion_bin>
  COMMAND rm -rf "${CMAKE_CURRENT_BINARY_DIR}/isodir"
  COMMAND cp -rp "${CMAKE_CURRENT_LIST_DIR}/iso_contents" "${CMAKE_CURRENT_BINARY_DIR}/isodir"
  COMMAND cp $<TARGET_FILE:stallion_bin> "${CMAKE_CURRENT_BINARY_DIR}/isodir/boot"
  COMMAND grub-mkrescue -o ${STALLION_ISO} "${CMAKE_CURRENT_BINARY_DIR}/isodir"
)
add_custom_target(stallion_iso ALL DEPENDS ${STALLION_ISO} stallion_bin)
add_custom_target(run_qemu
  COMMAND ${QEMU} -cdrom ${STALLION_ISO} -serial file:/dev/stdout
  DEPENDS stallion_iso)
