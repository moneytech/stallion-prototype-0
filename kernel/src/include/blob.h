#ifndef STALLION_BLOB_H
#define STALLION_BLOB_H
#include <stddef.h>

/**
 * A simple linked list of memory blobs. Used for file descriptors,
 * etc.
 */
typedef struct _stallion_blob {
  size_t total_size;
  size_t size;
  void* data;
  struct _stallion_blob* next;
} stallion_blob_t;

stallion_blob_t* stallion_blob_append(stallion_blob_t** head, void* data, size_t size);

#endif