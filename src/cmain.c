#include "settings/common_define.h"
#include "modules/kernel_modules.h"


unsigned char* FONT_BASE;

void _start() {
  // Mizink main entry point

  int font_segment = *(short*)(BOOT_LOAD + SECT_SIZE);
  int font_offset = *(short*)(BOOT_LOAD + SECT_SIZE + 2);
  int font_address = ((font_segment << 4) + font_offset) & 0xFFFFF;
  // Real-Modeのアドレス計算法を再現。
  FONT_BASE = (unsigned char*)font_address;

  draw_str(0, 0, 0x010F, "Hello world");

  while(1) {
  }
}
void draw_char(unsigned int x, unsigned int y, unsigned short color, unsigned char ch) {
  unsigned char* drawing_font_adr = FONT_BASE + (ch << 4);
  // 一つの文字あたり2^4 = 16byteなため
  unsigned char* drawing_vram_adr = VGA_IO_MEM_BASE + x + y * 80 * 16;
  // y座標が一つ下がるごとに幅(80)と一文字あたりの高さ(16)をかけた値を足す

  vga_set_read_plane(0x03);
  vga_set_write_plane(0x08);
  vram_font_copy(drawing_font_adr, drawing_vram_adr, 0x08, color);
  vga_set_read_plane(0x02);
  vga_set_write_plane(0x04);
  vram_font_copy(drawing_font_adr, drawing_vram_adr, 0x04, color);
  vga_set_read_plane(0x01);
  vga_set_write_plane(0x02);
  vram_font_copy(drawing_font_adr, drawing_vram_adr, 0x02, color);
  vga_set_read_plane(0x00);
  vga_set_write_plane(0x01);
  vram_font_copy(drawing_font_adr, drawing_vram_adr, 0x01, color);

  // ループアンローリング(笑)
}

void draw_str(unsigned int x,unsigned int y, unsigned short color, char* str) {
  char ch;
  while(ch = *str++) {
    draw_char(x, y, color, ch);
    x += 1;
    y += x / 80;
    x %= 80;
  }
}



