#include "descriptor.h"

void init_descriptor_tables() {
  init_gdt();
  init_idt();
}

void init_gdt() {

  gdt_ptr.limit = (sizeof(gdt_entry_t) * 7) - 1;
  gdt_ptr.base = gdt_entries;
  
  gdt_set_gate(0,0,0,0,0);
  gdt_set_gate(1,0,0,0x98,0x20);
  gdt_set_gate(2,0,0,0x92,0x20);
  gdt_set_gate(3,0,0,0xF8,0x20);
  gdt_set_gate(4,0,0,0xF2,0x20);
  
  gdt_set_tss((tss_entry64_t *)&gdt_entries[5], &tss, sizeof(tss),0x89,0);
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

void init_idt() {
  idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
  idt_ptr.base = idt_entries;

  idt_set_gate(0, (uint64_t)isr0, 0x08, 0x8E);
  idt_set_gate(1, (uint64_t)isr1, 0x08, 0x8E);
  idt_set_gate(2, (uint64_t)isr2, 0x08, 0x8E);
  idt_set_gate(3, (uint64_t)isr3, 0x08, 0x8E);
  idt_set_gate(4, (uint64_t)isr4, 0x08, 0x8E);
  idt_set_gate(5, (uint64_t)isr5, 0x08, 0x8E);
  idt_set_gate(6, (uint64_t)isr6, 0x08, 0x8E);
  idt_set_gate(7, (uint64_t)isr7, 0x08, 0x8E);
  idt_set_gate(8, (uint64_t)isr8, 0x08, 0x8E);
  idt_set_gate(9, (uint64_t)isr9, 0x08, 0x8E);
  idt_set_gate(10, (uint64_t)isr10, 0x08, 0x8E);
  idt_set_gate(11, (uint64_t)isr11, 0x08, 0x8E);
  idt_set_gate(12, (uint64_t)isr12, 0x08, 0x8E);
  idt_set_gate(13, (uint64_t)isr13, 0x08, 0x8E);
  idt_set_gate(14, (uint64_t)isr14, 0x08, 0x8E);
  idt_set_gate(15, (uint64_t)isr15, 0x08, 0x8E);
  idt_set_gate(16, (uint64_t)isr16, 0x08, 0x8E);
  idt_set_gate(17, (uint64_t)isr17, 0x08, 0x8E);
  idt_set_gate(18, (uint64_t)isr18, 0x08, 0x8E);
  idt_set_gate(19, (uint64_t)isr19, 0x08, 0x8E);
  idt_set_gate(20, (uint64_t)isr20, 0x08, 0x8E);
  idt_set_gate(21, (uint64_t)isr21, 0x08, 0x8E);
  idt_set_gate(22, (uint64_t)isr22, 0x08, 0x8E);
  idt_set_gate(23, (uint64_t)isr23, 0x08, 0x8E);
  idt_set_gate(24, (uint64_t)isr24, 0x08, 0x8E);
  idt_set_gate(25, (uint64_t)isr25, 0x08, 0x8E);
  idt_set_gate(26, (uint64_t)isr26, 0x08, 0x8E);
  idt_set_gate(27, (uint64_t)isr27, 0x08, 0x8E);
  idt_set_gate(28, (uint64_t)isr28, 0x08, 0x8E);
  idt_set_gate(29, (uint64_t)isr29, 0x08, 0x8E);
  idt_set_gate(30, (uint64_t)isr30, 0x08, 0x8E);
  idt_set_gate(31, (uint64_t)isr31, 0x08, 0x8E);

  idt_load(&idt_ptr);
}


void idt_set_gate(int num, uint64_t base, uint16_t sel, uint8_t flags) {
  idt_entries[num].base_low = base & 0xFFFF;
  idt_entries[num].base_middle = (base>>16) & 0xFFFF;
  idt_entries[num].base_high = (base>>32) & 0xFFFFFFFF;

  idt_entries[num].sel = sel;
  idt_entries[num].always0 = 0;
  idt_entries[num].flags = flags;
  idt_entries[num].zero = 0;
}

