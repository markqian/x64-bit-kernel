#include <stdint.h>

#define PAGE_SIZE 4096
#define WORD_SIZE 64
#define M_LIMIT 32768
#define USABLE_REGION 0x1
#define RESERVED_REGION 0x2

extern uint64_t kernel_start;
extern uint64_t kernel_end;

uint64_t kernel_size;
uint64_t bitmap[M_LIMIT];

typedef struct memory_info {
  uint64_t base;                      
  uint64_t length;                   
  uint32_t type;
  uint32_t reserved;
} mem_info; 


uint64_t pages_used;
uint64_t heap_size;
uint64_t heap_base;
uint64_t *alloc_page();
void free_page(uint64_t addr);
uint64_t *alloc_contigious_pages(int sz);
void free_contigious_pages(uint64_t addr, int sz);
void set_region(uint64_t base, uint64_t length);
void unset_region(uint64_t base, uint64_t length);
