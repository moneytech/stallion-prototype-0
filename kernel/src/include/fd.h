#ifndef STALLION_FD_H
#define STALLION_FD_H
#include "blob.h"
#include <stdbool.h>

typedef struct _stallion_fd {
  bool is_closed, is_terminal;
  stallion_blob_t *incoming, *outgoing;
  struct _stallion_fd *linked;
} stallion_fd_t;

#endif