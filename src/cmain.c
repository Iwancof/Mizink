#include "settings/common_define.h"

#ifndef INCLUDED_KERNEL_HEADER
#include "modules/kernel_modules.h"
#endif

int counter = 0;
char kernel_keyboard_task_name[] = "KEYBOARD:KERNEL";
char ascii_num[] = "0123456789ABCDEF";

void log(char* data);

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

  rk_draw_str(20, 0, 0x020F, "Mizink kernel loaded. Hello.");

  log("Load rust entry...");

  int value = rust_entry();

  log(format("Rust entry returns ... %d", value));
  log(format("char ! %c..", 'A'));

  unsigned int per_n_test;
  log("");
  log("");
  log("");
  log(format("Hello world %n", &per_n_test));
  log(format("Value is %d.", per_n_test));


  while(1) {
  }

  asm("cli");
  //current_task_name = task_name_tmp;
  asm("sti");
}

void ts_count_num() {
  counter += 1;
}

int log_counter = 1;
void log(char* data) {
  rk_draw_str(1, log_counter, 0x040F, data);
  log_counter++;
}


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

