#include "../kernel_modules.h"
#include "../../settings/common_define.h"

unsigned char KEY_BUFFER[KEY_BUFFER_LIMIT] = {0};
unsigned char read_point = 0, write_point = 0;
unsigned int KEYBUF_IS_READ = FALSE;

void  write_key_buf(char);

void int_keyboard(struct intr_frame *frame) {
  char k = inb(0x60);
  write_key_buf(k);
}

void write_key_buf(char x) {
  if(write_point + 1 == read_point) {
    kernel_panic();
    // バッファがいっぱい
  }
  KEY_BUFFER[write_point] = x;
  write_point += 1;  
}
char read_key_buf(void) {
  if(read_point == write_point) {
    KEYBUF_IS_READ = FALSE;
    return -1;
  }
  KEYBUF_IS_READ = TRUE;
  return KEY_BUFFER[read_point++];
}




