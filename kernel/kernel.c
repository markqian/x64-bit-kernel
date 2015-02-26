#include "descriptor.h"
#include "screen.h"

void main() {
  uint64_t in;

  init_gdt();
  init_idt();
  clear_screen();
  char s[10];
  s[0] = '1';
  s[1] = '1';
  s[2] = '1';
  s[3] = '1';
  s[4] = 0;
  print("Hello World!\n");
  print("I was here!\n");
  print(s);

  __asm__ volatile (
       "mov %rsp, %rsi\n\t"
       "movl $753664, %edi\n\t"
       "call	kprint_hex\n\t"       
  );

  kprint_hex(0xb8028, ((tss_entry64_t *)&gdt_entries[5])->base_low);
  kprint_hex(0xb8050, ((tss_entry64_t *)&gdt_entries[5])->base_low);
  kprint_hex(0xb8050+0x28, ((tss_entry64_t *)&gdt_entries[5])->base_low);
  kprint_hex(0xb8050+0x28+0x28, ((tss_entry64_t *)&gdt_entries[5])->base_low);

  __asm__ volatile("int $0x3");

 }
