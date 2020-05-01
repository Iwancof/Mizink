#include "../kernel_modules.h"

char *ascii = "0123456789ABCDEF";

void itoa(unsigned int num, char *dst) {
  char *st = dst;
  while(*(++dst));
  dst--;

  //rk_draw_char(0, 0, 0x010F, *dst);
  for(;st <= dst;dst--) {
    *dst = num % 10 + '0';
    num /= 10;
  }
}
void itoa_16(unsigned int num, char *dst) {
  char *st = dst;
  while(*(++dst));
  dst--;

  //rk_draw_char(0, 0, 0x010F, *dst);
  for(;st <= dst;dst--) {
    *dst = ascii[num % 16];
    num /= 16;
  }
}

