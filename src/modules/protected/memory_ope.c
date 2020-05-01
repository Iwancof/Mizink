/*
void* memcpy(void* dst, void* src, unsigned int len) {
  // 重複した領域には対応してない
  void* ret = dst;
  for(int i = 0;i < len;i++) {
    *(char*)dst = *(char*)src;
    dst++; src++;
  }
  return ret;
}
*/

#include "../kernel_modules.h"

void* memset(void* dst, char dt, unsigned int len) {
  rk_draw_str(0, 3, 0x010F, "call memset");
  void* ret = dst;
  for(int i = 0;i < len;i++) {
    *(char*)dst = dt;
    dst++;
  }
  return ret;
}
