#include "isr.h"

void isr_handler(registers_t regs) {
  print("recieved interrupt: ");
  uint64_t n = regs.rbp;
  printf("I was here %u", regs.rbp);
  print("\n");
}
