#ifndef STALLION_LIBC_STALLION_H
#define STALLION_LIBC_STALLION_H
#define STALLION_OS

#define STALLION_SYSCALL_EXIT 1
#define STALLION_SYSCALL_DECLARE_ATTRIBUTES 2
#define STALLION_SYSCALL_REQUEST_PERMISSIONS 3
#define STALLION_SYSCALL_GET_PID 4
// #define STALLION_SYSCALL_READ 2
// #define STALLION_SYSCALL_WRITE 3
// #define STALLION_SYSCALL_BLOCK 4

#define STALLION_ATTRIBUTE_FILESYSTEM 1

#define STALLION_PERMISSION_SERIAL_READ 1
#define STALLION_PERMISSION_SERIAL_WRITE 2

#define STALLION_RESULT_FAIL 0
#define STALLION_RESULT_OK 1

#ifdef __ASSEMBLY__
#else
#include "../stdint.h"

#if __STDC_VERSION__ >= 201112L
#define STALLION_NORETURN _Noreturn
#else
#ifdef __GNUC__
#define STALLION_NORETURN __attribute__((noreturn))
#else
#define STALLION_NORETURN
#endif
#endif

STALLION_NORETURN void stallion_syscall_exit(uint32_t exit_code);
int32_t stallion_syscall_declare_attributes(uint32_t attributes);
int32_t stallion_syscall_request_permissions(uint32_t permissions);
int32_t stallion_syscall_get_pid();

#endif

#endif