#include "low_level.h"

unsigned char inb(unsigned short port) {
  unsigned char result;
  __asm__("inb %%dx, %%al" : "=a" (result) : "d" (port));
  return result;
}

void outb(unsigned short port, unsigned char data) {
  __asm__("outb %%al, %%dx" : : "a" (data), "d" (port));
}

unsigned short inw(unsigned short port) {
  unsigned short result;
  __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
  return result;
}

void outw(unsigned short port, unsigned short data) {
  __asm__("out %%ax, %%dx" : : "a" (data), "d" (port));
}

void outsw (uint16_t usPort, void *buf, int count) {
  __asm__ ("cld; rep outsw"
	   :"=S" (buf), "=c" (count)
	   :"0" (buf), "1" (count), "d" (usPort)
	   :"flags");
}

void insl(int port, void *addr, int cnt)
{
asm volatile("cld; rep insl" :
"=D" (addr), "=c" (cnt) :
"d" (port), "0" (addr), "1" (cnt) :
"memory", "cc");
}

void insw (uint16_t usPort, void *buf, int count) {

   asm volatile ("cld; rep insw"
                 :"=c" (count), "=D" (buf)
                 :"0" (count), "1" (buf), "d" (usPort)
                 :"memory","flags");
}

void outl (uint16_t usPort, uint32_t ul) {

  asm volatile ("outl %0,%1"::"a" (ul), "Nd" (usPort));

}

uint32_t inl (uint16_t usPort)
{
   uint32_t ul; 
   asm volatile ("inl %1,%0":"=a" (ul):"Nd" (usPort));
   return ul;
}
