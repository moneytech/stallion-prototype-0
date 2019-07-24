include(ExternalProject)
include(ProcessorCount)
ProcessorCount(CORES)
include_directories(include)
add_library(stallion_userland OBJECT syscalls.c)

# Roll Perl 5.10
ExternalProject_Add(perl
  URL "http://www.cpan.org/src/5.0/perl-5.14.0.tar.gz"
  URL_HASH "MD5=e688b0ddad50bca9a6223693a85c439d"
  CONFIGURE_COMMAND
    cd <BINARY_DIR> && <SOURCE_DIR>/configure -des -Dprefix=<INSTALL_DIR>
  BUILD_COMMAND
    cd <BINARY_DIR> && make "-j${CORES}" && make install)
ExternalProject_Get_Property(perl install_dir)
set(PERL_BIN "${install_dir}/bin")
set(ENV{PATH} "${PERL_BIN}:$ENV{PATH}")

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
add_dependencies(automake perl)
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
add_dependencies(binutils automake autoconf)

# Status messages...
message(STATUS "Perl 5.10 bin dir: ${PERL_BIN}")
message(STATUS "Rolling autoconf 2.64: ${AUTOCONF}")
message(STATUS "Rolling automake 1.11.6: ${AUTOMAKE}")
message(STATUS "Rolling aclocal 1.11.6: ${ACLOCAL}")
