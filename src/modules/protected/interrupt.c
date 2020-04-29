#include "../../settings/common_define.h"
#include "../kernel_modules.h"

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
  set_interrupt(0, &zero_div);
}
