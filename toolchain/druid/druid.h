#ifndef DRUID_DRUID_H
#define DRUID_DRUID_H
#include <stdint.h>
#define DRUID_MAGIC 0xB16BAD
#define DRUID_VERSION_0 0x0
#define DRUID_FILE_TYPE_UNKNOWN 0x0
#define DRUID_FILE_TYPE_EXECUTABLE 0x1
#define DRUID_FILE_TYPE_LIBRARY 0x2

typedef union {
  uint16_t as_short;
  uint64_t as_long;
} druid_meta_value_t;

typedef struct _druid_meta {
  char *name;
  druid_meta_value_t value;
  struct _druid_meta *next;
} druid_meta_t;

typedef struct _druid_symbol {
  char *name;
  uint64_t size;
  void *data;
  struct _druid_symbol *next;
} druid_symbol_t;

typedef struct _druid_section {
  char *name;
  uint64_t size;
  void *data;
  struct _druid_section *next;
  druid_symbol_t* symbols;
} druid_section_t;

typedef struct {
  druid_meta_t *meta;
} druid_file_t;

#endif
