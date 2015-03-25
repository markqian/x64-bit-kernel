#include "isr.h"

void isr_handler(registers_t regs) {
  kprint("recieved interrupt: ");
  kprintf("I was here %x", regs.rbp);
  kprint("\n");
}

void irq_handler(registers_t regs) {
  if (regs.int_no != 0x20)
    kprintf("recieved interrupt: 0x%x\n",regs.int_no);

  if ((regs.int_no == 0x2f) || (regs.int_no == 0x2e)) {
    ide_irq_invoked = 1;
  }
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

void wait_for_irq15() {

  while (ide_irq_invoked != 1) {
    ;
  }

}
