#include "paging.h"

void *get_physaddr(void * virtualaddr) {
  //  uint64_t pml4index = (uint64_t)virtualaddr >> 

}

void map_pdp(void *physaddr, void *virutaladdr, uint64_t flags) {


}


void map_pd(void *physaddr, void *virutaladdr, uint64_t flags) {

}


void map_pt(void *physaddr, void *virutaladdr, uint64_t flags) {

}


void map_page(void *physaddr, void *virutaladdr, uint64_t flags) {
  //map correct entry in pml4
  //map correct entry in pdp
  //map correct entry in pd
  //map correct entry in pt


}

void unmap_page(void *virutaladdr) {
  
}

