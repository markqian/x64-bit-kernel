#include "gdt.h"

void init_gdt() {

  gdt_ptr.limit = (sizeof(gdt_entry_t) * 5) - 1;
  gdt_ptr.base = gdt_entries;
  
  gdt_set_gate(0,0,0,0,0);
  gdt_set_gate(1,0,0,0x98,0x20);
  gdt_set_gate(2,0,0,0x92,0x20);
  gdt_set_gate(3,0,0,0xF8,0x20);
  gdt_set_gate(4,0,0,0xF2,0x20);
  
  gdt_load(&gdt_ptr);

}


void gdt_set_gate(int num, u32int base, u32int limit, u8int access, u8int gran) {

  gdt_entries[num].base_low = (base & 0xFFFF);
  gdt_entries[num].base_middle = ((base >> 16) & 0xFF);
  gdt_entries[num].base_high = ((base >> 24) & 0xFF);

  gdt_entries[num].limit_low = (limit & 0xFFFF);
  gdt_entries[num].granularity = (limit >> 16) & 0x0F;

  gdt_entries[num].granularity |= gran & 0xF0;
  gdt_entries[num].access = access;

}
