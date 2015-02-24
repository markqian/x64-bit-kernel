#include <stdint.h>

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
  uint32_t RSP0_lower;
  uint32_t RSP0_higher;
  uint32_t RSP1_lower;
  uint32_t RSP1_higher;
  uint32_t RSP2_lower;
  uint32_t RSP2_higher;
  uint32_t reserved1;
  uint32_t reserved2;
  uint32_t IST_1_lower;
  uint32_t IST_1_higher;
  uint32_t IST_2_lower;
  uint32_t IST_2_higher;
  uint32_t IST_3_lower;
  uint32_t IST_3_higher;
  uint32_t IST_4_lower;
  uint32_t IST_4_higher;
  uint32_t IST_5_lower;
  uint32_t IST_5_higher;
  uint32_t IST_6_lower;
  uint32_t IST_6_higher;
  uint32_t IST_7_lower;
  uint32_t IST_7_higher;
  uint32_t reserved3;
  uint32_t reserved4;
  uint16_t MBA;
  uint16_t reserved5;
}__attribute__((packed));
typedef struct tss64 tss64_t;

struct gdt_ptr {
  uint16_t limit;
  gdt_entry_t *base;
} __attribute__((packed));
typedef struct gdt_ptr gdt_ptr_t;

extern void gdt_load(gdt_ptr_t *gdt_ptr);

void init_gdt();
void gdt_set_gate(int, uint32_t, uint32_t, uint8_t, uint8_t);
void gdt_set_tss(tss_entry64_t*, tss64_t*, uint32_t, uint8_t, uint8_t);

gdt_entry_t gdt_entries[7];
gdt_ptr_t gdt_ptr;
tss64_t tss;

