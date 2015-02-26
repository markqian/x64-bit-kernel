#include "descriptor.h"
#include "screen.h"

void main() {
  init_descriptor_tables();
  clear_screen();
  print("Hello World!\n");
  print("I was here!\n");

  __asm__ volatile("int $0x3\n\t");

 }
