#include <fcntl.h>

extern void exit(int code);
extern int main();

// TODO: Load environment, set handles, etc.
// TODO: argc, argv, etc.

void _start() {
  int ex = main();
  exit(ex);
}
