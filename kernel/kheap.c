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

uint64_t alloc_contigious_pages(int sz) {
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

  return addr;
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

uint64_t alloc_page() {
  int i = 0;
  uint64_t addr;

  i = find_free_page();

  if (i<0) {
    return 0;
  }

  set_bit(i);

  i *= PAGE_SIZE;

  addr = i;

  return addr;
}


void free_page(uint64_t addr) { 
  unset_bit(addr / PAGE_SIZE);
}

void init_physical_manager() {
  //find starting memory
  //parse memory map
  //init used pages
  

}
