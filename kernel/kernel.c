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

  printf("%x\n", m_info[0].type);
  kernel_size = (uint64_t)(&kernel_end) - (uint64_t)(&kernel_start);

 }
