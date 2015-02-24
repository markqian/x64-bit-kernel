#include "types.h"

struct gdt_entry {
  u16int limit_low;  
  u16int base_low; 
  u8int base_middle; 
  u8int access; 
  u8int granularity; 
  u8int base_high; 
} __attribute__((packed));
typedef struct gdt_entry gdt_entry_t;

struct tss_entry {
  u16int limit_low;  
  u16int base_low; 
  u8int base_middle; 
  u8int access; 
  u8int granularity; 
  u8int base_high; 
  u32int base_high2;
  u32int zero;
}__attribute__((packed));
typedef struct tss_entry tss_entry_t;

struct gdt_ptr {
  u16int limit;
  gdt_entry_t *base;
} __attribute__((packed));
typedef struct gdt_ptr gdt_ptr_t;

extern void gdt_load(gdt_ptr_t *gdt_ptr);

void init_gdt();
void gdt_set_gate(int, u32int, u32int, u8int, u8int);
void gdt_set_tss(tss_entry_t*, u32int, u8int, u8int);

gdt_entry_t gdt_entries[7];
gdt_ptr_t gdt_ptr;


