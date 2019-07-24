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
