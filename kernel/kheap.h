#include <stdint.h>

extern uint64_t kernel_start;
extern uint64_t kernel_end;


#define PAGE_SIZE 4096
#define WORD_SIZE 64
#define M_LIMIT 32768
uint64_t bitmap[M_LIMIT];

uint64_t pages_used;

uint64_t alloc_page();
void free_page(uint64_t addr);
uint64_t alloc_contigious_pages(int sz);
void free_contigious_pages(uint64_t addr, int sz);
