#include <stdint.h>

//low level stuff

unsigned char inb(unsigned short port);
unsigned short inw(unsigned short port);
uint32_t inl(uint16_t usPort);
void insl(int port, void *addr, int cnt);
void insw (uint16_t usPort, void *buf, int count);

void outb(unsigned short port, unsigned char data); 
void outw(unsigned short port, unsigned short data);
void outsw (uint16_t usPort, void *buf, int count);
void outl (uint16_t usPort, uint32_t ul);
