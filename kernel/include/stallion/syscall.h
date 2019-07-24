#ifndef STALLION_SYSCALL_H
#define STALLION_SYSCALL_H
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#define STALLION_SYSCALL_EXIT 0
#define STALLION_SYSCALL_CLOSE 1
#define STALLION_SYSCALL_EXECVE 2
#define STALLION_SYSCALL_FORK 3
#define STALLION_SYSCALL_FSTAT 4
#define STALLION_SYSCALL_GETPID 5
#define STALLION_SYSCALL_ISATTY 6
#define STALLION_SYSCALL_KILL 7
#define STALLION_SYSCALL_LINK 8
#define STALLION_SYSCALL_LSEEK 9
#define STALLION_SYSCALL_OPEN 10
#define STALLION_SYSCALL_READ 11
#define STALLION_SYSCALL_SBRK 12
#define STALLION_SYSCALL_STAT 13
#define STALLION_SYSCALL_TIMES 14
#define STALLION_SYSCALL_UNLINK 15
#define STALLION_SYSCALL_WAIT 16
#define STALLION_SYSCALL_WRITE 17
#define STALLION_SYSCALL_GETTIMEOFDAY 18

extern long stallion_syscall(long no, ...);

#ifdef STALLION_SUPPORT_ASYNC_SYSCALL
/**
 * The result of a Stallion system call.
 */
typedef struct {
  int errno;
  void *data;
  size_t size;
  void *context;
} stallion_async_syscall_result_t;

/*
 * Handles the result of a Stallion async_syscall.
 */
typedef void (*stallion_async_syscall_callback)(
    const stallion_async_syscall_result_t);

typedef enum {
  exit = 0,
  read = 1,
  write = 2,
  close = 3,
  create_channel = 4,
  get_pid = 5,
  yield_execution = 6
} stallion_async_syscall_number_t;

typedef struct {
  stallion_async_syscall_number_t number;
  bool blocking;
  void *arguments;
  size_t arguments_size;
  stallion_async_syscall_callback callback;
  void *context;
} stallion_async_syscall_t;

/*
 * Enqueues a Stallion system call.
 */
void stallion_async_syscall(const stallion_async_syscall_t);

#endif

#endif
