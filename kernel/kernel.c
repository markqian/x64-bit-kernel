#include "descriptor.h"
#include "screen.h"
#include "timer.h"

void kmain() {
  clear_screen();

  init_descriptor_tables();
  
  asm volatile("sti");
  
  init_timer(50);


  //  __asm__ volatile("int $0x13\n\t");

 }
