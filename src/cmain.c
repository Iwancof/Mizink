#include "settings/common_define.h"

#ifndef INCLUDED_KERNEL_HEADER
#include "modules/kernel_modules.h"
#endif

int counter = 0;
char kernel_keyboard_task_name[] = "KEYBOARD:KERNEL";

void mizink_main() {

  // Mizink main entry point
  asm("cli");
  vga_initialize(); 
  int_init();
  pic_init();

  //interrupt_initialize();
  //kbc_initialize();
  char *task_name_tmp = current_task_name;
  current_task_name = kernel_keyboard_task_name;
  asm("sti");

  //kernel_panic();
  
  rk_draw_str(0, 10, 0x010F, "Hello");
 
  rust_entry();

  while(1) {
  }

  asm("cli");
  //current_task_name = task_name_tmp;
  asm("sti");
}

void ts_count_num() {
  counter += 1;
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

float __mulsf3(float x, float y) {
  return 0.f;
}

float __fixsfsi(float x) {
  return x;
}

