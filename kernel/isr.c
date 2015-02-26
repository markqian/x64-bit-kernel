#include "isr.h"

void isr_handler(registers_t regs) {
  print("recieved interrupt: ");
  print("\n");
}
