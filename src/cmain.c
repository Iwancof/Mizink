#include "settings/common_define.h"
#include "modules/kernel_modules.h"

void _start() {
  // Mizink main entry point
  vga_initialize(); 

  draw_str(0, 0, 0x010F, "Hello world");

  while(1) {
  }
}



