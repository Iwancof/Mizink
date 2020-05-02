#include "../../settings/common_define.h"
#include "../kernel_modules.h" 
#define STI

void set_interrupt(int inm, void(*proc)(void)) {
  *(short*)(VECT_BASE + inm * 8 + 0) = (int)proc & 0x0000FFFF;
  *(short*)(VECT_BASE + inm * 8 + 2) = 0x8;
  *(short*)(VECT_BASE + inm * 8 + 4) = 0x8E00;
  *(short*)(VECT_BASE + inm * 8 + 6) = (int)proc >> 16;
}


void zero_div() {
  rk_draw_str(23, 25, 0x0004, "0 div");
  while(1) {
  }
}

void int_init() {
  // 割り込み初期設定
  set_interrupt(0, &zero_div);
  keyboard_init();
  set_interrupt(0x21, &asm_int_keyboard);
  timer_set(0.01f); // timeはミリ秒指定
  set_interrupt(0x20, &asm_int_timer);
}
void pic_init() {
  // PICの初期設定(割り込み許可もここ)
  outb(0x20, 0x11);
  outb(0x21, 0x20);
  outb(0x21, 0x04);
  outb(0x21, 0x05);
  outb(0x21, 0xFF);

  outb(0xA0, 0x11);
  outb(0xA1, 0x28);
  outb(0xA1, 0x02);
  outb(0xA1, 0x01);
  outb(0xA1, 0xFF);

  outb(0x21, 0b11111000);
}
