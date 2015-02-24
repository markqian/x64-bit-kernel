#include "screen.h"
#include "gdt.h"

void main() {
  init_gdt();
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

  kprint_hex(0xb8000, gdt_entries[4]);
 }
