#include "../kernel_modules.h"
#define IN_TIMER_COUNT 1193182.f

void int_timer(struct intr_frame *frame) {
}

void timer_set(float time) {
  outb(0x43, 0b00110100);
  int set_value = IN_TIMER_COUNT * time;
  outb(0x40, set_value & 0x00FF);
  outb(0x40, set_value >> 8);
}
