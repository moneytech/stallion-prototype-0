include(ExternalProject)
include(ProcessorCount)
ProcessorCount(CORES)
include_directories(include)
add_library(stallion_userland OBJECT syscalls.c)

# Roll Autoconf 2.64
ExternalProject_Add(autoconf
  URL "http://ftp.gnu.org/gnu/autoconf/autoconf-2.64.tar.gz"
  URL_HASH "MD5=30a198cef839471dd4926e92ab485361"
  CONFIGURE_COMMAND
    cd <BINARY_DIR> && <SOURCE_DIR>/configure --prefix <INSTALL_DIR>
  BUILD_COMMAND
    cd <BINARY_DIR> && make "-j${CORES}" && make install)
ExternalProject_Get_Property(autoconf install_dir)
set(AUTOCONF "${install_dir}/bin/autoconf")

# Roll Automake 1.11.6
ExternalProject_Add(automake
  URL "http://ftp.gnu.org/gnu/automake/automake-1.11.6.tar.gz"
  URL_HASH "MD5=0286dc30295b62985ca51919202ecfcc"
  CONFIGURE_COMMAND
    cd <BINARY_DIR> && <SOURCE_DIR>/configure --prefix <INSTALL_DIR>
  BUILD_COMMAND
    cd <BINARY_DIR> && make "-j${CORES}" && make install)
ExternalProject_Get_Property(automake install_dir)
set(AUTOMAKE_INSTALL_DIR ${install_dir})
set(AUTOMAKE "${install_dir}/bin/automake")
set(ACLOCAL "${install_dir}/bin/aclocal-1.11")

# Build binutils.
ExternalProject_Add(binutils
  SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/binutils-2.32"
  CONFIGURE_COMMAND
    cd <SOURCE_DIR>/ld && ${ACLOCAL} && ${AUTOMAKE}
)
add_dependencies(binutils automake)

# Status messages...
message(STATUS "Rolling autoconf 2.64: ${AUTOCONF}")
message(STATUS "Rolling automake 1.11.6: ${AUTOMAKE}")
message(STATUS "Rolling aclocal 1.11.6: ${ACLOCAL}")
