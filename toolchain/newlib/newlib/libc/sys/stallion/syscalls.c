#include "../../../../../../include/stallion/stallion.h"
#include <stdio.h>
#include <sys/errno.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/types.h>

void _exit() { stallion_syscall(STALLION_SYSCALL_EXIT); }

int close(int file) { return stallion_syscall(STALLION_SYSCALL_CLOSE, file); }

char **environ; /* pointer to array of char * strings that define the current
                   environment variables */

int execve(char *name, char **argv, char **env) {
  return stallion_syscall(STALLION_SYSCALL_EXECVE, name, argv, env);
}

int fork() { return stallion_syscall(STALLION_SYSCALL_FORK); }

int fstat(int file, struct stat *st) {
  return stallion_syscall(STALLION_SYSCALL_FSTAT, file, st);
}

int getpid() { return stallion_syscall(STALLION_SYSCALL_GETPID); }

int isatty(int file) { return stallion_syscall(STALLION_SYSCALL_ISATTY, file); }

int kill(int pid, int sig) {
  return stallion_syscall(STALLION_SYSCALL_KILL, pid, sig);
}

int link(char *old, char *new) {
  return stallion_syscall(STALLION_SYSCALL_LINK, old, new);
}

int lseek(int file, int ptr, int dir) {
  return stallion_syscall(STALLION_SYSCALL_LSEEK, file, ptr, dir);
}

int open(const char *name, int flags, ...) {
  // TODO: Support the rest of these arguments.
  return stallion_syscall(STALLION_SYSCALL_OPEN, name, flags);
}

int read(int file, char *ptr, int len) {
  return stallion_syscall(STALLION_SYSCALL_READ, file, ptr, len);
}

caddr_t sbrk(int incr) {
  return (caddr_t)stallion_syscall(STALLION_SYSCALL_SBRK, incr);
}

int stat(const char *file, struct stat *st) {
  return stallion_syscall(STALLION_SYSCALL_STAT, file, st);
}

clock_t times(struct tms *buf) {
  return stallion_syscall(STALLION_SYSCALL_TIMES, buf);
}

int unlink(char *name) {
  return stallion_syscall(STALLION_SYSCALL_UNLINK, name);
}

int wait(int *status) {
  return stallion_syscall(STALLION_SYSCALL_WAIT, status);
}

int write(int file, char *ptr, int len) {
  return stallion_syscall(STALLION_SYSCALL_WRITE, file, ptr, len);
}

int gettimeofday(struct timeval *p, struct timezone *z) {
  return stallion_syscall(STALLION_SYSCALL_GETTIMEOFDAY, p, z);
}
