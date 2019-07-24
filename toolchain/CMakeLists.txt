include(ExternalProject)
include(ProcessorCount)
ProcessorCount(CORES)
include_directories(include)
add_subdirectory(druid)
add_subdirectory(stld)
ExternalProject_Add(newlib
  SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/newlib
  CONFIGURE_COMMAND
    cd <BINARY_DIR> && <SOURCE_DIR>/configure
      --prefix=<INSTALL_DIR> --target=i686-stallion
  BUILD_COMMAND
    cd <BINARY_DIR> && make "-j${CORES}"
  INSTALL_COMMAND
    cd <BINARY_DIR> && make install)
#add_library(stallion_userland OBJECT syscalls.c)
