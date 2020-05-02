#include "../kernel_modules.h"

int t_id_max = 0;
char *current_task_name = 0;

void init_task(task_pool *tp) {
  tp->tasks[0].t_id = -1;
}
int subscribe(task_pool *tp, int (*fp)(event_args, char*)) {
  for(int i = 0;i < 16;i++) {
    if(tp->tasks[i].t_id == -1) {
      // ここに登録する
      tp->tasks[i].t_id = ++t_id_max;
      tp->tasks[i].event_fp = fp; // 登録
      tp->tasks[i + 1].t_id = -1; // 16を超えた時の処理はいつか追加する
      return t_id_max;
    }
  }
  // 登録できなかった
  rk_draw_str(20, 0, 0x010F, "TASK BUFFER FULL");
  kernel_panic();
}
void cancel(task_pool *tp, int canc_id) {
  int i;
  for(i = 0;i < 16;i++) {
    if(tp->tasks[i].t_id == canc_id) break;
  }
  if(i == 16) {
    // Not Found
    rk_draw_str(20, 0, 0x010F, "TASK NOT FOUND");
    kernel_panic(); 
  }
  for(;i < 15;i++) {
    tp->tasks[i] = tp->tasks[i + 1];
    // NULL以降も移動する(いつか直す)
  }
}
void broadcast(task_pool *tp, event_args ar) {
  for(int i = 0;i < 16;i++) {
    if(tp->tasks[i].t_id == -1) break;
    (tp->tasks[i].event_fp)(ar, current_task_name);
  }
}
