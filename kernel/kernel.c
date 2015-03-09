#include "descriptor.h"
#include "screen.h"
#include "timer.h"
#include "kheap.h"
#include "paging.h"

void kmain(mem_info *m_info, int n_entries) {
  clear_screen();
  init_descriptor_tables();
  init_physical_manager(m_info, n_entries);

  map((uint64_t)0xFFFF8aaaa0000000); 

  uint64_t * p = (uint64_t *) 0xFFFF8aaaa0000000;
 
  kprintf("0x%x\n", bitmap[0]);
  kprintf("0x%x\n", bitmap[1]);
  kprintf("0x%x\n", bitmap[2]);
  kprintf("0x%x\n", bitmap[3]);
  kprintf("0x%x\n", bitmap[4]);
  kprintf("0x%x\n", bitmap[5]);
  kprintf("0x%x\n", bitmap[6]);

 }
