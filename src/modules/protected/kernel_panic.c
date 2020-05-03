#ifndef INCLUDED_KERNEL_HEADER
#include "../kernel_modules.h"
#endif

_Noreturn void kernel_panic() {
  rk_draw_str(37, 20, 0x0400, "PANIC");

  do {
    rk_hlt();
  } while(1);
}
