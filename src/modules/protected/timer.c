#include "../kernel_modules.h"
#define IN_TIMER_COUNT 1193182.f

char kernel_timer_task_name[] = "TIMER:KERNEL";
task_pool timer_pool = {.tasks = {0} };
int kernel_timer_proc(event_args ar, char* s);


void int_timer(struct intr_frame *frame) {
  char *task_name_tmp = current_task_name;

  event_args ar = {.v = 0};
  // タイマーの関数に引数って必要？？
  broadcast(&timer_pool, ar);

  current_task_name = task_name_tmp;
}

void timer_set(float time) {
  init_task(&timer_pool);
  subscribe(&timer_pool, &kernel_timer_proc);

  outb(0x43, 0b00110100);
  int set_value = IN_TIMER_COUNT * time;
  outb(0x40, set_value & 0x00FF);
  // 下位ビット
  outb(0x40, set_value >> 8);
  // 上位ビット
}

int kernel_timer_proc(event_args ar, char* s) {
  //ts_count_num();
  // 
}
