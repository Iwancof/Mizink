unsigned char* FONT_BASE;

extern void outb(unsigned short port, unsigned char value);
extern void outw(unsigned short port, unsigned short value);
extern void outd(unsigned short port, unsigned int value);
// ƒAƒZƒ“ƒuƒŠ‚ÅŽÀ‘•‚³‚ê‚Ä‚é‚â‚Â‚ç


void vga_set_read_plane(unsigned char place);
void vga_set_write_plane(unsigned char plane);
void vram_font_copy(unsigned char* font_adr, unsigned char* vram_adr, unsigned char plane, unsigned short color);
void vga_initialize(); 


void draw_char(unsigned int x, unsigned int y, unsigned short color, unsigned char ch);
void draw_str(unsigned int x,unsigned int y, unsigned short color, char* str);


