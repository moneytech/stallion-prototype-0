#ifndef STLD_STLD_H
#define STLD_STLD_H
#include <stdbool.h>

typedef struct _stld_string_list {
  struct _stld_string_list *next;
  char *value;
} stld_string_list_t;

typedef struct {
  char *output_name;
  // The actual inputs
  stld_string_list_t *input_files;
  // Paths to .ld scripts
  stld_string_list_t *linker_scripts;
  // Names passed via -l
  stld_string_list_t *link_names;
  // Filenames found in directories passed via -L
  stld_string_list_t *possible_library_files;
  char *last_error;
  bool show_help;
} stld_options_t;

typedef enum { STLD_RESULT_OK, STLD_RESULT_FAIL } stld_result_t;

stld_result_t stld_string_list_append(stld_string_list_t **list, char *value);
void stld_string_list_destroy(stld_string_list_t *list);

/**
 * Reads default library paths, sets default output name, etc.
 */
stld_result_t stld_infer_default_options(stld_options_t *options);

stld_result_t stld_parse_arguments(int argc, char **argv,
                                   stld_options_t *options);

stld_result_t stld_link(stld_options_t *options);

#endif
