#include "isr.h"

struct gdt_entry {
  uint16_t limit_low;  
  uint16_t base_low; 
  uint8_t base_middle; 
  uint8_t access; 
  uint8_t granularity; 
  uint8_t base_high; 
} __attribute__((packed));
typedef struct gdt_entry gdt_entry_t;

struct tss_entry64 {
  uint16_t limit_low;  
  uint16_t base_low; 
  uint8_t base_middle; 
  uint8_t access; 
  uint8_t granularity; 
  uint8_t base_high; 
  uint32_t base_high2;
  uint32_t zero;
}__attribute__((packed));
typedef struct tss_entry64 tss_entry64_t;

struct tss64 {
  uint32_t reserved0;
  uint64_t rsp0;
  uint64_t rsp1;
  uint64_t rsp2;
  uint32_t reserved1;
  uint32_t reserved2;
  uint64_t ist[7];
  uint32_t reserved3;
  uint32_t reserved4;
  uint16_t reserved5;
  uint16_t ioMapBase;
}__attribute__((packed));
typedef struct tss64 tss64_t;

struct gdt_ptr {
  uint16_t limit;
  gdt_entry_t *base;
} __attribute__((packed));
typedef struct gdt_ptr gdt_ptr_t;

struct idt_entry {
  uint16_t base_low;
  uint16_t sel;
  uint8_t always0;
  uint8_t flags;
  uint16_t base_middle;
  uint32_t base_high;
  uint32_t zero;
}__attribute__((packed));
typedef struct idt_entry idt_entry_t;

struct idt_ptr {
  uint16_t limit;
  idt_entry_t *base;
}__attribute__((packed));
typedef struct idt_ptr idt_ptr_t;

gdt_entry_t gdt_entries[7];
gdt_ptr_t gdt_ptr;
idt_entry_t idt_entries[256];
idt_ptr_t idt_ptr;

tss64_t tss;
void init_descriptor_tables();

extern void idt_load(idt_ptr_t *idt_ptr);
void init_idt();
void idt_set_gate(int, uint64_t, uint16_t, uint8_t);

extern void gdt_load(gdt_ptr_t *gdt_ptr);

void init_gdt();
void gdt_set_gate(int, uint32_t, uint32_t, uint8_t, uint8_t);
void gdt_set_tss(tss_entry64_t*, tss64_t*, uint32_t, uint8_t, uint8_t);

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
