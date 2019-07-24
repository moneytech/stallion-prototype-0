include(ExternalProject)
include(ProcessorCount)
ProcessorCount(CORES)
include_directories(include)
set(ENV{PATH} "${CMAKE_CURRENT_LIST_DIR}/local/bin:$ENV{PATH}")
ExternalProject_Add(newlib
  SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/newlib
  CONFIGURE_COMMAND
    cd <BINARY_DIR> && <SOURCE_DIR>/configure
      --prefix=<INSTALL_DIR> --target=i686-stallion
  BUILD_COMMAND
    cd <BINARY_DIR> && make "-j${CORES}"
  INSTALL_COMMAND
  cd <BINARY_DIR> && make install && cp -rp <INSTALL_DIR>/i686-stallion "${CMAKE_CURRENT_LIST_DIR}/sysroot")
ExternalProject_Get_Property(newlib install_dir)
set(NEWLIB_INSTALL_DIR ${install_dir}/i686-stallion)
set(NEWLIB_INCLUDE_DIRS ${NEWLIB_INSTALL_DIR}/include)
set(NEWLIB_LIBRARY_DIRS ${NEWLIB_INSTALL_DIR}/lib)
ExternalProject_Add(binutils
  SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/binutils-2.32
  CONFIGURE_COMMAND
    cd <BINARY_DIR> && <SOURCE_DIR>/configure
      --prefix=<INSTALL_DIR> --target=i686-stallion
      --with-sysroot="${CMAKE_CURRENT_LIST_DIR}/sysroot"
      --disable-werror
      CFLAGS=-I${NEWLIB_INCLUDE_DIRS}
      LDFLAGS=-L${NEWLIB_LIBRARY_DIRS}
  BUILD_COMMAND
    cd <BINARY_DIR> && make "-j${CORES}"
  INSTALL_COMMAND
  cd <BINARY_DIR> && make install)
add_dependencies(binutils newlib)
add_subdirectory(druid)
add_subdirectory(stld)
#add_library(stallion_userland OBJECT syscalls.c)
