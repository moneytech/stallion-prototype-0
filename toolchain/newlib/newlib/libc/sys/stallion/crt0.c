#include <fcntl.h>

// TODO: Init environment.
extern void exit(int code);
extern int main();

void _start() {
  int ex = main();
  exit(ex);
}
