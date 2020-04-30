#define INCLUDED_KERNEL_HEADER

struct intr_frame {
    unsigned int rip;
    unsigned int cs;
    unsigned int rflags;
    unsigned int rsp;
    unsigned int ss;
};

extern unsigned char* FONT_BASE;
extern unsigned char* PANIC_MESSAGE;

// アセンブリで実装されてるやつら
extern void outb(unsigned short port, unsigned char value);
extern void outw(unsigned short port, unsigned short value);
extern void outd(unsigned short port, unsigned int value);
extern unsigned char inb(unsigned short port);
extern unsigned short inw(unsigned short port);
extern unsigned int ind(unsigned short port);
extern void rk_hlt();
extern void set_idtr(int);
extern void asm_int_keyboard();

void vga_set_read_plane(unsigned char place);
void vga_set_write_plane(unsigned char plane);
void vram_font_copy(unsigned char* font_adr, unsigned char* vram_adr, unsigned char plane, unsigned short color);
void vga_initialize(); 
void set_interrupt(int interrupt_number, void(*proc)(void));
void int_init();
void pic_init();
void int_keyboard(struct intr_frame*);

// raw kernel
void rk_draw_char(unsigned int x, unsigned int y, unsigned short color, unsigned char ch);
void rk_draw_str(unsigned int x,unsigned int y, unsigned short color, char* str);

// Rustのためのインターフェース
int rust_entry();
void kernel_panic();

// 試験的な関数
void ts_draw_num_10(unsigned int x);
void ts_draw_num_16(unsigned int x);
void ts_count_num();
// テストコード
