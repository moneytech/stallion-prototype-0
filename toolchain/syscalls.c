#include <errno.h>
#include <stallion/syscall.h>
#undef errno
extern int errno;

int _exit() 
