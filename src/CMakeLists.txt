set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -nostdlib")
add_subdirectory(${CMAKE_SYSTEM_PROCESSOR})
add_executable(stallion_bin
  $<TARGET_OBJECTS:stallion_arch>
)
target_link_options(stallion_bin PUBLIC -nostdlib -lgcc -T "${CMAKE_CURRENT_LIST_DIR}/linker.ld")
set_target_properties(stallion_bin PROPERTIES PREFIX "")
set_target_properties(stallion_bin PROPERTIES OUTPUT_NAME "stallion")
set_target_properties(stallion_bin PROPERTIES SUFFIX ".bin")

