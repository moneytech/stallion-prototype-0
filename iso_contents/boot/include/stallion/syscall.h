#ifndef STALLION_SYSCALL_H
#define STALLION_SYSCALL_H
#include <stdbool.h>
#include <stddef.h>

/**
 * The result of a Stallion system call.
 */
typedef struct {
  int errno;
  void* data;
  size_t size;
  void* context;
} stallion_syscall_result_t;

/*
 * Handles the result of a Stallion syscall.
 */
typedef void (*stallion_syscall_callback)(const stallion_syscall_result_t);

typedef enum {
  exit = 0,
  read = 1,
  write = 2,
  close = 3,
  create_channel = 4,
  get_pid = 5,
  yield_execution = 6
} stallion_syscall_number_t;

typedef struct {
  stallion_syscall_number_t number;
  bool blocking;
  void* arguments;
  size_t arguments_size;
  stallion_syscall_callback callback;
  void* context;
} stallion_syscall_t;

/*
 * Enqueues a Stallion system call.
 */
void stallion_syscall(const stallion_syscall_t);

#endif
