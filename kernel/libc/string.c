#include <string.h>

void *memcpy(void *restrict s1, const void *restrict s2, size_t n) {
  char *_s1 = s1;
  const char *_s2 = s2;
  for (size_t i = 0; i < n; i++) {
    _s1[i] = _s2[i];
  }
  return s1;
}

void *memmove(void *s1, const void *s2, size_t n) {
  unsigned char *_s1 = (unsigned char *)s1;
  const unsigned char *_s2 = (const unsigned char *)s2;
  if (_s1 < _s2) {
    for (size_t i = 0; i < n; i++)
      _s1[i] = _s2[i];
  } else {
    for (size_t i = n; i != 0; i--)
      _s1[i - 1] = _s2[i - 1];
  }
  return s1;
}