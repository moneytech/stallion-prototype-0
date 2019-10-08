#ifndef STALLION_FILE_H
#define STALLION_FILE_H

typedef enum {
  STALLION_FENT_FILE,
  STALLION_FENT_DIRECTORY,
  STALLION_FENT_LINK,
  STALLION_FENT_MOUNT
} stallion_fent_type_t;

typedef struct _stallion_fent {
  stallion_fent_type_t type;
  const char* name;
  struct _stallion_fent *next;
} stallion_fent_t;

#endif