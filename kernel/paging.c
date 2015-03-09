#include "paging.h"
#include "kheap.h"
#include "screen.h"

uint64_t get_pml4_index(uint64_t virtualaddr) {
  uint64_t entry;
  entry = (virtualaddr & PML4_MASK);
  entry /= 0x8000000000;
  if (virtualaddr & PML4_HIGH) {
    entry += 0x100;
  }

  return entry;
}

uint64_t get_pdp_index(uint64_t virtualaddr) {
  uint64_t entry;

  entry = (virtualaddr & PDP_MASK) / 0x40000000;
  return entry;
}

uint64_t get_pd_index(uint64_t virtualaddr) {
  uint64_t entry;

  entry = (virtualaddr & PD_MASK) / 0x200000;
  return entry;
}

uint64_t get_pt_index(uint64_t virtualaddr) {
  uint64_t entry;

  entry = (virtualaddr & PT_MASK) / 0x1000;
  return entry;
}

uint64_t * get_pdp_table(uint64_t pml4_entry) {  
  return (uint64_t *) ((uint64_t)0xFFFFFF7FBFC00000 + (uint64_t)pml4_entry * (uint64_t)0x1000);
}

uint64_t *get_pd_table(uint64_t pml4_entry, uint64_t pdp_entry) {
  return (uint64_t *) 
    ((uint64_t)0xFFFFFF7F80000000 + 
     (uint64_t)pml4_entry * (uint64_t)0x200000 + 
     (uint64_t)pdp_entry * (uint64_t)0x1000);
}

uint64_t *get_pt_table(uint64_t pml4_entry, uint64_t pdp_entry, uint64_t pd_entry) {
  return (uint64_t *) 
    ((uint64_t)0xFFFFFF0000000000 + 
     (uint64_t)pml4_entry * (uint64_t)0x40000000 + 
     (uint64_t)pdp_entry * (uint64_t)0x200000 + 
     (uint64_t)pd_entry * (uint64_t)0x1000);
}

void map(uint64_t virtualaddr) {
  uint64_t *pml4_table;
  uint64_t *pdp_table;
  uint64_t *pd_table;
  uint64_t *pt_table;
  uint64_t *freepage;

  uint64_t pml4_entry;
  uint64_t pdp_entry;
  uint64_t pd_entry;
  uint64_t pt_entry;

  pml4_table = (uint64_t *) 0xFFFFFF7FBFDFE000;
  //get entry number in table
  pml4_entry = get_pml4_index(virtualaddr);
  //check if page present not in PML4 table
  if (!(pml4_table[pml4_entry] & 0x3)) {
    // alloc a page
    pdp_table = alloc_page();
    // set pml4 table to this address
    pml4_table[pml4_entry] = ((uint64_t)pdp_table | 0x3);    
  } 

  //if page present then move on to pdp
  pdp_table = get_pdp_table(pml4_entry);
  //get entry number in table
  pdp_entry = get_pdp_index(virtualaddr);
  //check if empty
  if (!(pdp_table[pdp_entry] & 0x3)) {
    // alloc a page
    pd_table = alloc_page();
    // set pdp table to this address
    pdp_table[pdp_entry] = ((uint64_t)pd_table | 0x3);
  }

  //set up pd table
  pd_table = get_pd_table(pml4_entry, pdp_entry);
  //get pd index
  pd_entry = get_pd_index(virtualaddr);
  //check if empty
  if (!(pd_table[pd_entry] & 0x3)) {
    pt_table = alloc_page();
    pd_table[pd_entry] = ((uint64_t)pt_table | 0x3);
  }

  //get pt table
  pt_table = get_pt_table(pml4_entry, pdp_entry, pd_entry);
  //get pt index
  pt_entry = get_pt_index(virtualaddr);
  //check if empty
  if (!(pt_table[pt_entry] & 0x3)) {
    freepage = alloc_page();
    pt_table[pt_entry] = ((uint64_t)freepage | 0x3);
  }

}
