#include "isr.h"

void isr_handler(registers_t regs) {
  kprint("recieved interrupt: ");
  kprintf("I was here %x", regs.rbp);
  kprint("\n");
}

void irq_handler(registers_t regs) {
  kprint("recieved interrupt: ");

  if (regs.int_no >= 40) {
    outb(0xA0, 0x20);
  }

  outb(0x20, 0x20);

  if (interrupt_handlers[regs.int_no] != 0) {
    isr_t handler = interrupt_handlers[regs.int_no];
    handler(regs);
  }
}

void register_interrupt_handler(uint8_t n, isr_t handler) {
  interrupt_handlers[n] = handler;
}
