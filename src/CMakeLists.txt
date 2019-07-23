# Compile/linking options... Compiler specific, to some degree.
# set(CMAKE_LINKER_FLAGS "-nostdlib -T ${CMAKE_CURRENT_LIST_DIR}/linker.ld")

# gcc opts
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -nostdlib")
# list(APPEND CMAKE_ASM_FLAGS ${CMAKE_C_FLAGS})

# Use gcc to compile assembly

add_subdirectory(${CMAKE_SYSTEM_PROCESSOR})
add_executable(stallion_bin
  $<TARGET_OBJECTS:stallion_arch>
)


target_link_libraries(stallion_bin gcc -nostdlib -T ${CMAKE_CURRENT_LIST_DIR}/linker.ld)

# Set output to stallion.bin instead of just "stallion" or "stallion.exe"
set_target_properties(stallion_bin PROPERTIES PREFIX "")
set_target_properties(stallion_bin PROPERTIES OUTPUT_NAME "stallion")
set_target_properties(stallion_bin PROPERTIES SUFFIX ".bin")
