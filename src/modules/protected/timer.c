#include "../kernel_modules.h"
#define IN_TIMER_COUNT 1193182.f

void int_timer(struct intr_frame *frame) {
  char r = read_key_buf();
  if(KEYBUF_IS_READ == TRUE) {
    ts_draw_num_16((int)r);
  }
  char buf[] = "--------";
  itoa(read_point, buf);
  rk_draw_str(0, 0, 0x010F, buf);
}

void timer_set(float time) {
  outb(0x43, 0b00110100);
  int set_value = IN_TIMER_COUNT * time;
  outb(0x40, set_value & 0x00FF);
  outb(0x40, set_value >> 8);
}
