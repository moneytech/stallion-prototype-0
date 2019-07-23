#include <stallion.h>

void interrupt_handler(uint8_t type) {
  kwrites("Got an interrupt: ");
  kputi(type);
  kputc('\n');
}
