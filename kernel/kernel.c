#include "descriptor.h"
#include "screen.h"
#include "timer.h"
#include "kheap.h"
#include "paging.h"
#include "ide_controller.h"
#include "timer.h"
/* The default and seemingly universal sector size for CD-ROMs. */
#define ATAPI_SECTOR_SIZE 2048
 
/* The default ISA IRQ numbers of the ATA controllers. */
#define ATA_IRQ_PRIMARY     0x0E
#define ATA_IRQ_SECONDARY   0x0F
 
/* The necessary I/O ports, indexed by "bus". */
#define ATA_DATA(x)         (x)
#define ATA_FEATURES(x)     (x+1)
#define ATA_SECTOR_COUNT(x) (x+2)
#define ATA_ADDRESS1(x)     (x+3)
#define ATA_ADDRESS2(x)     (x+4)
#define ATA_ADDRESS3(x)     (x+5)
#define ATA_DRIVE_SELECT(x) (x+6)
#define ATA_COMMAND(x)      (x+7)
#define ATA_DCR(x)          (x+0x206)   /* device control register */
 
/* valid values for "bus" */
#define ATA_BUS_PRIMARY     0x1F0
#define ATA_BUS_SECONDARY   0x170
/* valid values for "drive" */
#define ATA_DRIVE_MASTER    0xA0
#define ATA_DRIVE_SLAVE     0xB0
 
/* ATA specifies a 400ns delay after drive switching -- often
 * implemented as 4 Alternative Status queries. */
#define ATA_SELECT_DELAY(bus) \
  {inb(ATA_DCR(bus));inb(ATA_DCR(bus));inb(ATA_DCR(bus));inb(ATA_DCR(bus));}

void sleep(uint64_t t) {
  uint64_t start = tick;
  while ((tick - start) < t) {
    ;
  }
}

struct PRD {
  uint32_t addr;
  uint16_t bsize;
  uint16_t reserved;
}__attribute__((packed));
typedef struct PRD PRD;

void kmain(mem_info *m_info, int n_entries) {
  clear_screen();
  init_descriptor_tables();
  init_physical_manager(m_info, n_entries);
  init_timer(50);
  uint8_t read_cmd[12] = { 0xA8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  uint8_t status;
  int size = 0;
  ide_irq_invoked = 0;
  asm volatile("sti");
  uint16_t r;
  uint16_t bar4;
  int bus, device, i, function;
  /* uint16_t mspace[128]; */
  /* for (bus = 0; bus<256; bus++) { */
  /*   for (device = 0; device<32; device++) { */
  /*     for (function = 0; function <8; function++) { */
  /* 	r = pciConfigReadWord(bus, device, function,0); */
  /* 	if (r != 0xffff) {	  */
  /* 	  bar4  = pciConfigReadWord(bus,device,function,0x2); */
  /* 	  kprintf("bus %d device %d func %d 0x%x\n",bus,device,function,bar4); */
  /* 	} */
  /*     } */
  /*   } */
  /* } */
  
  pciConfigWriteWord(0,1,1,4,0x107);
  bar4  = pciConfigReadWord(0,1,1,0x4);
  kprintf("bus %d device %d func %d 0x%x\n",0,1,1,bar4);

  outb(0xc040+0x0, 0);
  outb(0xc040+0x8, 0);

  /* for (i = 0; i<128; i++) { */
  /*   mspace[i]  = pciConfigReadWord(0, 1,1,i*2); */
  /*   kprintf("0x%x ", mspace[i]); */
  /* } */
  /* kprintf("\n\n"); */

  /* for (i = 0; i<128; i++) { */
  /*   mspace[i]  = pciConfigReadWord(0, 1,0,i*2); */
  /*   kprintf("0x%x ", mspace[i]); */
  /* } */
  /* kprintf("\n\n"); */

  /* for (i = 0; i<128; i++) { */
  /*   mspace[i]  = pciConfigReadWord(0, 2,0,i*2); */
  /*   kprintf("0x%x ", mspace[i]); */
  /* } */

  /* ide_initialize(0x1F0, 0x3F6, 0x170, 0x376, 0x000); */

  PRD *k = (PRD*)0x2000;
  k->addr = (uint64_t)alloc_contigious_pages(2);
  k->bsize = 0x1000 * 2;
  k->reserved = 0;

  outl(0xc040+0xC, 0x2000);
  outl(0xc040+0x4, 0x2000);
  kprintf("%x\n", *((uint64_t*)(k->addr + 0)));

  /* unsigned char b1, b2,b3,b4,s; */
  /* b1 = 0x00; */
  /* b2 = 0x20; */
  /* b3 = 0x00; */
  /* b4 = 0x00; */
  
  /* kprintf("%x\n",b1); */
  /* kprintf("%x\n",b2); */
  /* kprintf("%x\n",b3); */
  /* kprintf("%x\n",b4); */
 
  /* outb(0x8, 8); */

  /* outb(0xC, b1);  */
  /* outb(0xD, b2);  */
  /* outb(0xE, b3);  */
  /* outb(0xF, b4);  */
  
  outb (0x176, 0xA0 | (0 << 4));
  sleep(2);
  /* ATA_SELECT_DELAY (0x170); */
  outb (0x171, 1);
  sleep(2);
  outb (0x174, (2048 & 0xff));
  sleep(2);
  outb (0x175, (2048 >> 8));
  sleep(2);
  outb (0x177, 0xA0);
  sleep(2);

  while ((status = inb (0x177)) & 0x80) {
    asm volatile ("pause");
  }

  while (!((status = inb (0x177)) & 0x8) && !(status & 0x1)) {
    asm volatile ("pause");
  }

  /* outb(0xc040+0x2, 32); */
  /* outb(0xc040+0xA, 32); */

  unsigned char s = inb(0xc040 +0x2);
  unsigned char s2 = inb(0xc040 +0xA);
  
  kprintf("status: %x\n",s);
  kprintf("status: %x\n",s2);

  /* kprintf("status %d\n", status); */

  read_cmd[9] = 1;              /* 1 sector */
  read_cmd[2] = (0x1b >> 0x18) & 0xFF;   /* most sig. byte of LBA */
  read_cmd[3] = (0x1b >> 0x10) & 0xFF;
  read_cmd[4] = (0x1b >> 0x08) & 0xFF;
  read_cmd[5] = (0x1b >> 0x00) & 0xFF;   /* least sig. byte of LBA */

  outsw (ATA_DATA (0x170), (uint16_t *) read_cmd, 6);

  outb(0xc040+0x0, 1);
  outb(0xc040+0x8, 1);

  wait_for_irq15();
  uint16_t st1, st2;
  st1 = inb(0xc040+0x2);
  st2 = inb(0xc040+0xA);

  kprintf("bus master status %d\n", st1);
  kprintf("bus master status %d\n", st2);

  /* int ks; */
  /* for (ks = 0; ks<100; ks+=4)   */
  kprintf("%x\n", *((uint64_t*)(k->addr + 0)));
  size = (((int) inb (ATA_ADDRESS3 (0x170))) << 8) |
    (int) (inb (ATA_ADDRESS2 (0x170)));

  /* status = inb (ATA_COMMAND (170)); */
  map((uint64_t)0xFFFF8aaaa0000000);

  uint64_t * p = (uint64_t *) 0xFFFF8aaaa0000000;
 
  insw (ATA_DATA (0x170), ide_buf, size / 2);
  kprintf("0x%x\n", size);
  kprintf("%d\n", status);
  uint64_t * ss = ide_buf;
  kprintf("0x%x\n", *ss);
  kprintf("0x%x\n", ide_buf[2]);
  kprintf("0x%x\n", ide_buf[3]);
  kprintf("0x%x\n", ide_buf[4]);
  kprintf("0x%x\n", ide_buf[5]);

  /* kprintf("0x%x\n", bitmap[5]); */
  /* kprintf("0x%x\n", bitmap[6]); */

 }
