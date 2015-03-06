#include <stdint.h>

struct page {
  
  uint64_t present : 1;
  uint64_t rw : 1;
  uint64_t user : 1;
  uint64_t accessed : 1;
  uint64_t dirty : 1;
  uint64_t unused : 7;
  uint64_t frame : 52;

} __attribute__((packed)); 
typedef struct page page_t;
