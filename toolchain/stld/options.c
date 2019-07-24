#include "stld.h"
#include <stdlib.h>
#include <string.h>

#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
#include <dirent.h>
#include <stdio.h>
#include <sys/types.h>
#else
#error "stld only can be built for Unix as of now."
#endif

stld_result_t stld_extract_search_paths(char *path_str,
                                        stld_options_t *options);

stld_result_t stld_infer_default_options(stld_options_t *options) {
  if (options == NULL)
    return STLD_RESULT_FAIL;

  // Default to "a.out"
  options->output_name = "a.out";
  options->last_error = "Unhandled error occurred.";

  // Next, search for library paths in the environment.
  options->input_files = NULL;
  options->link_names = NULL;
  options->possible_library_files = NULL;
  options->show_help = false;

  return STLD_RESULT_OK;
}

stld_result_t stld_parse_arguments(int argc, char **argv,
                                   stld_options_t *options) {
  for (int i = 0; i < argc; i++) {
    char *arg = argv[i];
    if (i < argc - 1) {
      if (strcmp(arg, "-o") == 0) {
        if (options->output_name != NULL &&
            (strcmp(options->output_name, "a.out") != 0)) {
          char *next = argv[i + 1];
          options->output_name = next;
          i++;
        } else {
          options->last_error = "Output name has already been set.";
          return STLD_RESULT_FAIL;
        }
      }
    }

    if ((strcmp(arg, "-h") == 0) || (strcmp(arg, "--help") == 0)) {
      options->show_help = true;
      return STLD_RESULT_OK;
    }
    if (strncmp(arg, "-L", 2) == 0 && strlen(arg) > 2) {
      stld_result_t result = stld_extract_search_paths(&arg[2], options);
      if (result != STLD_RESULT_OK) {
        options->last_error = "Error occurred when adding an input file.";
        return result;
      }
    } else if (strncmp(arg, "-l", 2) == 0 && strlen(arg) > 2) {
      stld_result_t result =
          stld_string_list_append(&options->link_names, &arg[2]);
      if (result != STLD_RESULT_OK) {
        options->last_error = "Error occurred when adding a library name.";
        return result;
      }
    } else if (strlen(arg) > 0 && strncmp(arg, "-", 1) != 0) {
      stld_result_t result =
          stld_string_list_append(&options->input_files, arg);
      if (result != STLD_RESULT_OK) {
        options->last_error = "Error occurred when adding an input file.";
        return result;
      }
    } else {
      options->last_error = "Invalid argument.";
      return STLD_RESULT_FAIL;
    }
  }

  // Next, extract search paths.
  char *search_paths[4];
  search_paths[0] = getenv("LD_LIBRARY_PATH");
  search_paths[1] = "/usr/local/lib";
  search_paths[2] = "/lib";
  search_paths[3] = "/usr/lib";

  for (int i = 0; i < (sizeof(search_paths) / sizeof(char *)); i++) {
    stld_result_t result = stld_extract_search_paths(search_paths[i], options);
    if (result != STLD_RESULT_OK)
      return result;
  }

  return STLD_RESULT_OK;
}

stld_result_t stld_extract_search_paths(char *path_str,
                                        stld_options_t *options) {
  if (path_str == NULL)
    return STLD_RESULT_OK;
  char *path = strtok(path_str, ":");
  while (path != NULL) {
    DIR *dp = opendir(path);
    if (dp != NULL) {
      struct dirent *ep;
      while ((ep = readdir(dp))) {
        if (ep->d_type == DT_REG || ep->d_type == DT_LNK) {
          stld_result_t result = stld_string_list_append(
              &options->possible_library_files, ep->d_name);
          if (result == STLD_RESULT_FAIL) {
            options->last_error =
                "Out of memory when searching linker search paths.";
            return STLD_RESULT_FAIL;
          }
        }
      }
      closedir(dp);
    }
    path = strtok(NULL, ":");
  }
  return STLD_RESULT_OK;
}

stld_result_t stld_string_list_append(stld_string_list_t **list, char *value) {
  size_t name_len = strlen(value);
  stld_string_list_t *str =
      (stld_string_list_t *)malloc(sizeof(stld_string_list_t));
  if (str == NULL)
    return STLD_RESULT_FAIL;
  str->value = malloc(name_len + 1);
  strcpy(str->value, value);
  str->value[name_len] = 0;
  str->next = NULL;
  if (*list == NULL) {
    *list = str;
  } else {
    stld_string_list_t *ptr = list[0];
    while (ptr->next != NULL) {
      ptr = ptr->next;
    }
    ptr->next = str;
  }
  return STLD_RESULT_OK;
}

void stld_string_list_destroy(stld_string_list_t *list);
