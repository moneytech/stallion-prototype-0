#ifndef STLD_STLD_H
#define STLD_STLD_H

typedef struct _stld_string_list {
  struct _stld_string_list* next;
  char* value;
} stld_string_list_t;

typedef struct {
  char* output_name;
  stld_string_list_t input_files;
  stld_string_list_t linker_scripts;
  stld_string_list_t link_libraries;
  stld_string_list_t link_directories;
  char* last_error;
} stld_options_t;

typedef enum {
  STLD_RESULT_OK,
  STLD_RESULT_FAIL
} stld_result_t;

/**
 * Reads default library paths, sets default output name, etc.
 */
stld_result_t stld_infer_default_options(stld_options_t* options);

stld_result_t stld_link(stld_options_t* options);

#endif
