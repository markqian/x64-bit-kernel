#include "descriptor.h"
#include "screen.h"
#include "timer.h"
#include "kheap.h"


typedef struct memory_info {
  uint64_t base;                      
  uint64_t length;                   
  uint32_t type;
  uint32_t reserved;
} mem_info; 


void kmain(mem_info *m_info, int n_entries) {
  clear_screen();

  //init kernel variables
  uint64_t kernel_size;
  uint64_t load_address;
  init_descriptor_tables();
  kernel_size = (uint64_t)&kernel_end - (uint64_t)&kernel_start;

  uint64_t p = alloc_contigious_pages(1000); 
  p = alloc_contigious_pages(10);
  uint64_t k = bitmap[15];
  kprintf("0x%x\n", k);

 }
