#include "settings/define.c"

extern void outb(unsigned short port, unsigned char value);	// assembly
extern void outw(unsigned short port, unsigned short value);
extern void outd(unsigned short port, unsigned int value);

union xreg {
  struct {
    unsigned char low;
    unsigned char high;
  };
  unsigned short x;
};

void _start() {
  // Mizink main entry point

  union xreg value = {.x = 0x0702};
  unsigned short port = 0x03C4;

  outw(port, value.x);

  *(unsigned char*)(0xA0000 + 0) = 0xFF;
  value.high = 0x04;
  outw(port, value.x);

  *(unsigned char*)(0xA0000 + 1) = 0xFF;
  value.high = 0x02;
  outw(port, value.x);

  *(unsigned char*)(0xA0000 + 2) = 0xFF;
  value.high = 0x01;
  outw(port, value.x);
 
  *(unsigned char*)(0xA0000 + 3) = 0xFF;

  while(1) {
  }
}

