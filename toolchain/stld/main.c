#include "stld.h"
#include <stdio.h>

int main(int argc, char **argv) {
  stld_options_t options;
  stld_result_t result = stld_infer_default_options(&options);
  if (result != STLD_RESULT_OK) {
    fputs(options.last_error, stderr);
    return 1;
  }

  result = stld_parse_arguments(argc, argv, &options);
  if (result != STLD_RESULT_OK) {
    fputs(options.last_error, stderr);
    return 1;
  }

  puts("Parsed.");
  return 0;
}
