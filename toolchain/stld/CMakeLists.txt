add_library(stld
  elf.h stld.h
  options.c 
)

add_executable(stld_exe main.c)
target_link_libraries(stld_exe stld)
set_target_properties(stld_exe PROPERTIES OUTPUT_NAME "stld")
