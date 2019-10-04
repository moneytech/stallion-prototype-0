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

char *strcpy(char *restrict s1, const char *restrict s2) {
  return strncpy(s1, s2, strlen(s2) + 1);
}

char *strncpy(char *restrict s1, const char *restrict s2, size_t n) {
  for (size_t i = 0; i < n; i++) {
    s1[i] = s2[i];
  }
  return s1;
}

char *strcat(char *restrict s1, const char *restrict s2) {
  return strncat(s1, s2, strlen(s2));
}

char *strncat(char *restrict s1, const char *restrict s2, size_t n) {
  size_t idx = strlen(s1);
  for (size_t i = 0; i < n; i++) {
    s1[idx + i] = s2[i];
  }
  s1[idx + n] = 0;
  return s1;
}

int memcmp(const void *s1, const void *s2, size_t n) {
  const char *_s1 = s1, *_s2 = s2;
  for (size_t i = 0; i < n; i++) {
    if (_s1[i] < _s2[i])
      return -1;
    else if (_s1[i] > _s2[i])
      return 1;
  }
  return 0;
}

int strcmp(const char *s1, const char *s2) {
  return memcmp(s1, s2, strlen(s1));
}

int strcoll(const char *s1, const char *s2) {
  char t1[1 + strxfrm(0, s1, 0)];
  strxfrm(t1, s1, sizeof(t1));
  char t2[1 + strxfrm(0, s2, 0)];
  strxfrm(t2, s2, sizeof(t2));
  return strcmp(t1, t2);
}

int strncmp(const char *s1, const char *s2, size_t n) {
  return memcmp(s1, s2, n);
}

size_t strxfrm(char *restrict s1, const char *restrict s2, size_t n) {
  size_t n2 = strlen(s2);
  if (n > n2)
    strcpy(s1, s2);
  return n2;
}

void *memchr(const void *s, int c, size_t n) {
  unsigned char *p = (unsigned char *)s;
  while (n--)
    if (*p != (unsigned char)c)
      p++;
    else
      return p;
  return NULL;
}

char *strchr(const char *s, int c) { return memchr(s, c, strlen(s) + 1); }

size_t strcspn(const char *s1, const char *s2) {
  size_t ret = 0;
  while (*s1)
    if (strchr(s2, *s1))
      return ret;
    else
      s1++, ret++;
  return ret;
}

char *strpbrk(const char *s1, const char *s2) {
  while (*s1)
    if (strchr(s2, *s1++))
      return (char *)--s1;
  return 0;
}

char *strrchr(const char *s, int c) {
  const char *ret = 0;
  do {
    if (*s == (char)c)
      ret = s;
  } while (*s++);
  return (char *)ret;
}

size_t strspn(const char *s1, const char *s2) {
  size_t ret = 0;
  while (*s1 && strchr(s2, *s1++))
    ret++;
  return ret;
}

char *strstr(const char *s1, const char *s2) {
  size_t n = strlen(s2);
  while (*s1)
    if (!memcmp(s1++, s2, n))
      return (char *)s1 - 1;
  return 0;
}

char *strtok(char *restrict s1, const char *restrict s2) {
  static char *p = 0;
  if (s1)
    p = s1;
  else if (!p)
    return 0;
  s1 = p + strspn(p, s2);
  p = s1 + strcspn(s1, s2);
  if (p == s1)
    return p = 0;
  p = *p ? *p = 0, p + 1 : 0;
  return s1;
}

void *memset(void *s, int c, size_t n) {
  char *_s = s;
  for (size_t i = 0; i < n; i++) {
    _s[i] = c;
  }
  return s;
}

char *strerror(int errnum) {
  // TODO: Proper strerror
  return errnum ? "Error" : "No error";
}

size_t strlen(const char *s) {
  size_t out = 0;
  while (s != NULL) {
    out++;
    s++;
  }
  return out;
}