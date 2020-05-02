
#include "../../settings/common_define.h"
#ifndef INCLUDED_KERNEL_HEADER
#include "../kernel_modules.h"
#endif

unsigned char* FONT_BASE = 0;
unsigned char PANIC_MESSAGE[0xFF] = {0};

union xreg {
  struct {
    unsigned char low;
    unsigned char high;
  };
  unsigned short x;
};


void vga_set_read_plane(unsigned char plane) {
  union xreg ax = {.high = plane & 3, .low = 0x04 };
  outw(VGA_GRA_ADR, ax.x);
}
void vga_set_write_plane(unsigned char plane) {
  union xreg ax = {.high = plane & 15, .low = 0x02 };
  outw(VGA_SEQ_ADR, ax.x);
}

void vram_font_copy(unsigned char* font_adr, unsigned char* vram_adr, unsigned char plane, unsigned short _color) { // colorは上位8bitがバック、下位8bitがフォワードになっている
  union xreg color = { .x = _color };
  unsigned char back_mask = ((color.high & plane) == 0) - 1;
  // バックグラウンドが選択プレーンに該当するならback_mask = 0xFF
  unsigned char forward_mask = ((color.low & plane) == 0) - 1;
  // フォワードが選択プレーンに該当するならforward_mask = 0xFF

  for(int i = 0;i < 16;i++) {
    unsigned char font_data = *font_adr;
    // フォワードデータ
    unsigned char anti_font_data = ~font_data;
    // バックデータ

    font_adr += 1; // フォントアドレスを一段下げる

    if((_color & 0b00010000) == 0) {
      // 普通(透過じゃない)
      anti_font_data &= back_mask;
    } else {
      // 透過する
      anti_font_data &= *vram_adr; // 透過するならすでにある情報をとってくる
    }
    font_data |= anti_font_data; //描画データを完成させる
    
    *vram_adr = font_data;

    vram_adr += 80;
    // 一つ下のラインへ
  }
}

void vram_bit_copy(unsigned char bit_pattern, unsigned char* vram_adr, unsigned char plane, unsigned short col) {
  char back_mask = ((plane & (col & 0x0F)) == 0) - 1;
  // 選択中のプレーンが色(フォワード)と一致しているかどうか
  // 該当するなら(plane & (col & 0xF))が0じゃないのでback_mask = 0xFF
  char anti_pattern = ~bit_pattern;
  *vram_adr = (back_mask & bit_pattern) | (anti_pattern & *vram_adr);
  // 既存のデータと合わせて出力
}

void vga_initialize() {
  int font_segment = *(short*)(BOOT_LOAD + SECT_SIZE);
  int font_offset = *(short*)(BOOT_LOAD + SECT_SIZE + 2);
  int font_address = ((font_segment << 4) + font_offset) & 0xFFFFF;
  // Real-Modeのアドレス計算法を再現。
  FONT_BASE = (unsigned char*)font_address;
}

