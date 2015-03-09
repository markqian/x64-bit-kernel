#include <stdint.h>

#define PT_MASK 0x1FFFFF
#define PD_MASK 0x3FFFFFFF
#define PDP_MASK 0x7FFFFFFFFF
#define PML4_HIGH 0xFFFF000000000000
#define PML4_MASK 0x7FFFFFFFFFFF

#define P_SIZE 0x1000
#define PT_SIZE 0x200000
#define PD_SIZE 0x40000000

uint64_t get_pdp_index(uint64_t virutaladdr);
uint64_t get_pd_index(uint64_t virtualaddr);
uint64_t get_pt_index(uint64_t virtualaddr);
uint64_t get_pml4_index(uint64_t virtualaddr);

void map(uint64_t virtualaddr);

struct entry {
  
  uint64_t present : 1;
  uint64_t rw : 1;
  uint64_t user : 1;
  uint64_t accessed : 1;
  uint64_t dirty : 1;
  uint64_t unused : 7;
  uint64_t frame : 52;

} __attribute__((packed)); 
typedef struct entry entry_t;

