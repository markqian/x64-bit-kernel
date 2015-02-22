
struct gdt_entry {
  unsigned short limit_low; 
  unsigned short base_low; 
  unsigned char base_middle; 
  unsigned char access; 
  unsigned char granularity; 
  unsigned char base_high; 
} __attribute__((packed));
typedef struct gdt_entry gdt_entry_t;

struct gdt_ptr {
  unsigned short limit;
  gdt_entry_t *base;
} __attribute__((packed));
typedef struct gdt_ptr gdt_ptr_t;

extern void gdt_flush(unsigned int);

static void init_gdt();
static void gdt_set_gate(int, unsigned int, unsigned int, unsigned char, unsigned char);

gdt_entry_t gdt_entries[5];
gdt_ptr_t gdt_ptr;


