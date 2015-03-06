#include <stdarg.h>
#include <stdint.h>

// video device constants
#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0f

// screen device io ports
#define REG_SCREEN_CTRL 0x3D4
#define REG_SCREEN_DATA 0x3D5


void print_char(char character, int col, int row, char attribute_byte);
int get_screen_offset(int col, int row);
int get_cursor();
void set_cursor(int offset);
void print_at(char * message, int col, int row);
void kprint(char *message);
void kputc(char c);
void kprintf(char *fmt, ...);
void clear_screen();
void memory_copy(char * source, char * dest, int no_bytes);
int handle_scrolling(int cursor_offset);
char * itoa(int value, char * str, int base);
