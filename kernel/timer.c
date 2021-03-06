#include "timer.h"
#include "isr.h"
#include "screen.h"
#include "low_level.h"


void timer_callback(registers_t regs) {
  tick++;
  /* kprintf("Tick: %d\n", tick);   */
}

void init_timer(uint64_t frequency) {
  tick = 0;
  register_interrupt_handler(IRQ0, &timer_callback);

  uint64_t divisor = 1193180 / frequency;

  outb(0x43, 0x36);
  uint8_t l = (uint8_t)(divisor & 0xFF);
  uint8_t h = (uint8_t)( (divisor>>8) & 0xFF );

  outb(0x40, l);
  outb(0x40, h);
}
