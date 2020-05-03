#include "../kernel_modules.h"
#include "../../settings/common_define.h"

task_pool keyboard_pool = {.tasks = {0} };
int kernel_keyboard_proc(event_args ar, char* s);

int id;

void keyboard_init() {
  //ts_draw_num_16((int)&keyboard_pool);
  init_task(&keyboard_pool);
  id = subscribe(&keyboard_pool, &kernel_keyboard_proc);
}

int kernel_keyboard_proc(event_args ar, char* s) {
  /*
  rk_draw_str(0, 0, 0x010F, s);
  ts_draw_num_16(ar.v);
  cancel(&keyboard_pool, id);
  */
}

void int_keyboard(struct intr_frame *frame) {
  char k = inb(0x60);
  event_args ar = { .v = k };
  broadcast(&keyboard_pool, ar);
}





