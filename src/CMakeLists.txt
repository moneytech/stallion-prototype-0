add_subdirectory(${CMAKE_SYSTEM_PROCESSOR})
add_executable(stallion_bin
  $<TARGET_OBJECTS:stallion_arch>
)

# Compile/linking options... Compiler specific, to some degree.
set(CMAKE_LINKER_FLAGS "-nostdlib -T ${CMAKE_CURRENT_LIST_DIR}/linker.ld")

# gcc opts
if(${CMAKE_C_COMPILER} MATCHES "gcc")
  add_compile_options("-ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -nostdlib")
  set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -lgcc")
elseif(${CMAKE_C_COMPILER} MATCHES "clang")
  add_compile_options("-ffreestanding -fno-builtin -nostdlib -nostdinc -nostdinc++")
endif()

# Set output to stallion.bin instead of just "stallion" or "stallion.exe"
set_target_properties(stallion_bin PROPERTIES PREFIX "")
set_target_properties(stallion_bin PROPERTIES OUTPUT_NAME "stallion")
set_target_properties(stallion_bin PROPERTIES SUFFIX ".bin")
