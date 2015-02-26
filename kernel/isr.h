#include <stdint.h>

typedef struct registers {
  uint64_t ds;
  uint64_t rax, rcx, rdx, rbx, rsp, rbp, rsi, rdi;
  uint64_t int_no, err_code;
  uint64_t rip, cs, eflags, userrsp, ss;

} registers_t;

void isr_handler(registers_t regs);
