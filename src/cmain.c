#include "settings/common_define.h"

#ifndef INCLUDED_KERNEL_HEADER
#include "modules/kernel_modules.h"
#endif

void div();

void _start() {
  // Mizink main entry point
  int_init();

  vga_initialize(); 
  //interrupt_initialize();
  //kbc_initialize();

  rust_entry();
  
  while(1) {
  }
}

char ascii_num[] = "0123456789ABCDEF";

void ts_draw_num_10(unsigned int x) {
  char buf[] = "------------";
  for(int i = 11;0 <= i;i--) {
    buf[i] = ascii_num[x % 10];
    x /= 10;
  }
  rk_draw_str(0, 10, 0x010F, buf);
}

void ts_draw_num_16(unsigned int x) {
  char buf[] = "--------";
  for(int i = 7;0 <= i;i--) {
    buf[i] = ascii_num[x % 16];
    x /= 16;
  }
  if(x != 0) {
    kernel_panic();
  }
  rk_draw_str(0, 10, 0x010F, buf);
}

