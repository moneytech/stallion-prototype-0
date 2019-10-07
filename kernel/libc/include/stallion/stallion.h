#ifndef STALLION_LIBC_STALLION_H
#define STALLION_LIBC_STALLION_H
#define STALLION_OS
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

#endif