#ifndef STALLION_LIBC_STALLION_H
#define STALLION_LIBC_STALLION_H
#define STALLION_OS
#include "../stdint.h"

void stallion_syscall_exit(uint32_t exit_code);

#endif