#include <stdint.h>
#include "ide_controller.h"

#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

typedef struct registers {
  uint64_t ds;
  uint64_t rax, rcx, rdx, rbx, rsp, rbp, rsi, rdi;
  uint64_t int_no, err_code;
  uint64_t rip, cs, eflags, userrsp, ss;

} registers_t;

void isr_handler(registers_t regs);
void irq_handler(registers_t regs);

typedef void (*isr_t)(registers_t);
void register_interrupt_handler(uint8_t n, isr_t handler);
isr_t interrupt_handlers[256];

unsigned char ide_irq_invoked;
void wait_for_irq15();
