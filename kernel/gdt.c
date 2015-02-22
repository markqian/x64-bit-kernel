#include "gdt.h"


static void init_gdt() {
  gdt_ptr.limit = (sizeof(gdt_entry_t) * 5) - 1;
  gdt_ptr.base = gdt_entries;
  
  gdt_set_gate(0,0,0,0,0);
  gdt_set_gate(1,0,0xFFFFFFFF,0x9A,0xCF);
  gdt_set_gate(2,0,0xFFFFFFFF,0x92,0xCF);
  gdt_set_gate(3,0,0xFFFFFFFF,0xFA,0xCF);
  gdt_set_gate(4,0,0xFFFFFFFF,0xF2,0xCF);
    
}


static void gdt_set_gate(int num, unsigned base, unsigned limit, unsigned char access, unsigned char gran) {
  




}
