#include <stallion.h>

stallion_blob_t* stallion_blob_append(stallion_blob_t** head, void* data, size_t size) {
  stallion_blob_t* p = kmalloc(sizeof(stallion_blob_t));
  if (p != NULL) {
    // Allocate a new pointer for the given blob.
    void *data_ptr = kmalloc(size);
    if (data_ptr == NULL) {
      kfree(p);
      p = NULL;
    } else {
      // COPY the data into the blob.
      // Set the necessary fields.
      kmemcpy(data_ptr, data_ptr, size);
      p->data = data_ptr;
      p->size = size;
      p->next = NULL;

      // Next, set the head.
      if (*head == NULL) {
        p->total_size = size;
        *head = p;
      } else {
        stallion_blob_t* cur = *head;
        while (cur->next != NULL) {
          cur = cur->next;
        }
        cur->next = p;
      }
    }
  }
  return p;
}