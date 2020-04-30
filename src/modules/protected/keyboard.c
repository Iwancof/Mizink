#include "../kernel_modules.h"
int y = 0;

void int_keyboard(struct intr_frame *frame) {
  (void)frame;
  
  inb(0x60);

  ts_count_num();

  outb(0x20, 0x20);
}




