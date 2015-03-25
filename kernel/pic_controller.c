#include "pic_controller.h"

void PIC_sendEOI(unsigned char irq) {
  if(irq >= 8)
    outb(PIC2_COMMAND,PIC_EOI);
  
  outb(PIC1_COMMAND,PIC_EOI);
}

uint16_t pciConfigReadWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
  uint32_t address;
  uint32_t lbus = (uint32_t)bus;
  uint32_t lslot = (uint32_t)slot;
  uint32_t lfunc = (uint32_t)func;
  uint16_t tmp = 0;
  
  address = (uint32_t)((lbus << 16) | (lslot << 11) |
		       (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
  outl(0xCF8, address);
  tmp = (uint16_t)((inl (0xCFC) >> ((offset & 2) * 8)) & 0xffff);
  return tmp;

}

void pciConfigWriteWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint16_t data) {
  uint32_t address;
  uint32_t lbus = (uint32_t)bus;
  uint32_t lslot = (uint32_t)slot;
  uint32_t lfunc = (uint32_t)func;
  uint16_t tmp = 0;
  
  address = (uint32_t)((lbus << 16) | (lslot << 11) |
		       (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
  outl(0xCF8, address);
  outl(0xCFC, data);
}

uint16_t pciCheckVendor(uint8_t bus, uint8_t slot) {
  uint16_t vendor, device;
  if ((vendor = pciConfigReadWord(bus,slot,0,0)) != 0xFFFF) {

    device = pciConfigReadWord(bus, slot,0,2);

  }

  return vendor;
}

/* void checkDevice(uint8_t bus, uint8_t device) { */
/*   uint8_t function = 0; */
  
/*   vendorID = getVendorID(bus, device, function); */
/*   if (vendorID = 0xFFFF) return; */
/*   checkFunction(bus, device, function); */
/*   headerType = getHeaderType(bus, device, function); */
/*   if ((headerType & 0x80) != 0) { */
/*     for (function = 1; function < 0; function++) { */
/*       if (getVenorID(bus, device, function) != 0xFFFF) { */
/* 	checkFunction(bus, device, function); */
/*       } */
/*     } */
/*   } */
/* } */

/* void checkFunction(uint8_t bus, uint8_t device, uint8_t function) { */
  

/* } */

/* void checkAllBuses() { */
/*   uint8_t bus; */
/*   uint8_t device; */

/*   for (bus = 0; bus < 256; bus++) { */
/*     for (device = 0; device < 32; device++) { */
/*       checkDevice(bus, device); */
/*     } */
/*   } */

/* } */
