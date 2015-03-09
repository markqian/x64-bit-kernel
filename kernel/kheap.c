#include "kheap.h"

void set_bit(uint64_t bit) {
  uint64_t mask;
  mask = 1;
  bitmap[bit / 64] |=  (mask << (bit % 64));
}

void unset_bit(uint64_t bit) {
  uint64_t mask;
  mask = 1;
  bitmap[bit / 64] &= ~(mask << (bit % 64));
}

int find_contigious_pages(int sz) {
  uint64_t i;
  uint64_t bits;
  int bitpos;
  int nfound;

  i = 0;
  nfound = 0;
  while (i < M_LIMIT) {

    while (bitmap[i] == 0xFFFFFFFFFFFFFFFF) {
      i++;
    }

    bits = bitmap[i];
    for (bitpos = 0; bitpos<64; bitpos++) {
      if ((bits & 1) != 0) {
	nfound = 0;
	bits >>= 1;
      }
      else {
	nfound++;
	if (sz == nfound) {

	  i = i * WORD_SIZE + bitpos - sz + 1;

	  return i;
	}
      }
    }
    i++;   
  }

  return -1;
}

uint64_t *alloc_contigious_pages(int sz) {
  uint64_t i,j;
  uint64_t addr;

  i = find_contigious_pages(sz);

  if (i<0)
    return 0;

  for (j =0; j<sz; j++) {
    set_bit(i+j);
  }
  
  i *= PAGE_SIZE;

  addr = i;

  return (uint64_t*)addr;
}

void free_contigious_pages(uint64_t addr, int sz) {
  int i;
  for (i = 0; i<sz; i++) {
    unset_bit(addr / PAGE_SIZE);
    addr += PAGE_SIZE;
  }
}


int find_free_page() {
  int i = 0;
  uint64_t bits;
  int bitpos;

  while (bitmap[i] == 0xFFFFFFFFFFFFFFFF) {
    i++;
    if ( i == M_LIMIT) //assume M_LIMIT is max memory
      return -1;
  }

  bits  = bitmap[i];
  bitpos = 0;

  while ((bits & 1) != 0) {
    bits >>= 1;
    bitpos++;
  }

  i *= WORD_SIZE;
  i += bitpos;
  
  return i;
}

uint64_t *alloc_page() {
  int i = 0;
  uint64_t addr;

  i = find_free_page();

  if (i<0) {
    return 0;
  }

  set_bit(i);

  i *= PAGE_SIZE;

  addr = i;

  return (uint64_t*)addr;
}


void free_page(uint64_t addr) { 
  unset_bit(addr / PAGE_SIZE);
}

void set_region(uint64_t base, uint64_t length) {
  int i;

  if (length < PAGE_SIZE) {
    set_bit(base/PAGE_SIZE);
    pages_used += 1;
  }
  else {
    for (i=0; i<length/PAGE_SIZE; i++) {      
      set_bit(base/PAGE_SIZE + i);
    }
    pages_used += length/PAGE_SIZE;

  }
}


void unset_region(uint64_t base, uint64_t length) {
  int i;

  if (length < PAGE_SIZE) {
    unset_bit(base/PAGE_SIZE);
    pages_used -= 1;
  }
  else {
    for (i=0; i<length/PAGE_SIZE; i++) {
      unset_bit(base/PAGE_SIZE + i);
    }
    pages_used -= length/PAGE_SIZE;
  }
}

void set_rest(uint64_t base) {
  int i;

  for (i=base/PAGE_SIZE; i<M_LIMIT; i++) {
    bitmap[i] = 0xffffffffffffffff;
  }
}


void init_physical_manager(mem_info *m_info, int n_entries) {
  int i;
  //initialize the kernel size variable.
  kernel_size = (uint64_t)&kernel_end - (uint64_t)&kernel_start;
  pages_used = 0;

  for (i = 0; i<n_entries; i++) {

    if (m_info[i].type == USABLE_REGION) {

      //the region we are interested in
      if (m_info[i].base == 0x100000) {
	//not interested in region below 1MB so set it as used. 
	set_region(0, 0x100000); 
	//set kernel region as used.
	set_region(m_info[i].base, kernel_size);
	//set rest of the bitmap as used since it's not being used.
	set_rest(m_info[i].base+m_info[i].length);
	
	heap_base = 0x100000;
	heap_size = m_info[i].length;
      }

    }
  }
}
