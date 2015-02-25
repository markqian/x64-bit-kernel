#include "descriptor.h"

void init_gdt() {

  gdt_ptr.limit = (sizeof(gdt_entry_t) * 7) - 1;
  gdt_ptr.base = gdt_entries;
  
  gdt_set_gate(0,0,0,0,0);
  gdt_set_gate(1,0,0,0x98,0x20);
  gdt_set_gate(2,0,0,0x92,0x20);
  gdt_set_gate(3,0,0,0xF8,0x20);
  gdt_set_gate(4,0,0,0xF2,0x20);
  
  gdt_set_tss(&gdt_entries[5], &tss, sizeof(tss),0,0);
  gdt_load(&gdt_ptr);

}

void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {

  gdt_entries[num].base_low = (base & 0xFFFF);
  gdt_entries[num].base_middle = ((base >> 16) & 0xFF);
  gdt_entries[num].base_high = ((base >> 24) & 0xFF);

  gdt_entries[num].limit_low = (limit & 0xFFFF);
  gdt_entries[num].granularity = (limit >> 16) & 0x0F;

  gdt_entries[num].granularity |= gran & 0xF0;
  gdt_entries[num].access = access;

}

void gdt_set_tss(tss_entry64_t* tss_entry, tss64_t* tss, uint32_t limit, uint8_t access, uint8_t gran) {
  tss_entry->base_low = ((uint64_t)tss & 0xFFFF);
  tss_entry->base_middle = (((uint64_t)tss >> 16) & 0xFF);
  tss_entry->base_high = (((uint64_t)tss >> 24) & 0xFF);

  tss_entry->limit_low = (limit & 0xFFFF);
  tss_entry->granularity = (limit >> 16) & 0x0F;

  tss_entry->granularity |= gran & 0xF0;
  tss_entry->access = access;

  tss_entry->base_high2 = (((uint64_t)tss >> 32) & 0xFFFFFFFF);
  tss_entry->zero = 0;
}
