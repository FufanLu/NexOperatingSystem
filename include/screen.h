#ifndef SCREEN_H
#define SCREEN_H

#define VIDEO_ADDRESS   0xb8000
#define MAX_ROWS        25
#define MAX_COLS        80
#define WHITE_ON_BLACK  0x0f
#define GREEN_ON_BLACK  0x0a
#define CYAN_ON_BLACK   0x0b
#define RED_ON_BLACK    0x0c
#define YELLOW_ON_BLACK 0x0e

/* Screen I/O ports */
#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

/* Public API */
void clear_screen(void);
void print(const char *message);
void print_at(const char *message, int col, int row);
void print_color(const char *message, unsigned char color);
void print_char(char c, int col, int row, unsigned char attr);
void set_cursor_offset(int offset);
int  get_cursor_offset(void);
int  get_offset(int col, int row);
int  get_offset_row(int offset);
int  get_offset_col(int offset);

#endif
