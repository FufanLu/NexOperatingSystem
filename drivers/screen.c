/* ============================================================
 * Screen Driver
 * Handles VGA text-mode output (80x25, color)
 * Writes directly to video memory at 0xb8000
 * ============================================================ */

#include "../include/screen.h"
#include "../include/ports.h"

/* ---- Internal helpers ---- */

static int handle_scrolling(int cursor_offset);

/* Print a single character at (col, row) with attribute byte */
void print_char(char c, int col, int row, unsigned char attr) {
    unsigned char *vidmem = (unsigned char *) VIDEO_ADDRESS;
    if (!attr) attr = WHITE_ON_BLACK;

    int offset;
    if (col >= 0 && row >= 0) {
        offset = get_offset(col, row);
    } else {
        offset = get_cursor_offset();
    }

    if (c == '\n') {
        row = get_offset_row(offset);
        offset = get_offset(0, row + 1);
    } else if (c == '\b') {
        /* Backspace: move cursor back and clear character */
        if (offset >= 2) {
            offset -= 2;
            vidmem[offset] = ' ';
            vidmem[offset + 1] = attr;
        }
        set_cursor_offset(offset);
        return;
    } else {
        vidmem[offset] = c;
        vidmem[offset + 1] = attr;
        offset += 2;
    }

    offset = handle_scrolling(offset);
    set_cursor_offset(offset);
}

/* Print a null-terminated string at current cursor position */
void print(const char *message) {
    int i = 0;
    while (message[i] != 0) {
        print_char(message[i], -1, -1, WHITE_ON_BLACK);
        i++;
    }
}

/* Print a string at a specific (col, row) position */
void print_at(const char *message, int col, int row) {
    int offset;
    if (col >= 0 && row >= 0) {
        offset = get_offset(col, row);
        set_cursor_offset(offset);
    }
    int i = 0;
    while (message[i] != 0) {
        print_char(message[i], -1, -1, WHITE_ON_BLACK);
        i++;
    }
}

/* Print with a specific color attribute */
void print_color(const char *message, unsigned char color) {
    int i = 0;
    while (message[i] != 0) {
        print_char(message[i], -1, -1, color);
        i++;
    }
}

/* Clear the entire screen */
void clear_screen(void) {
    unsigned char *vidmem = (unsigned char *) VIDEO_ADDRESS;
    int screen_size = MAX_COLS * MAX_ROWS;
    for (int i = 0; i < screen_size; i++) {
        vidmem[i * 2] = ' ';
        vidmem[i * 2 + 1] = WHITE_ON_BLACK;
    }
    set_cursor_offset(get_offset(0, 0));
}

/* ---- Cursor manipulation via VGA ports ---- */

int get_cursor_offset(void) {
    port_byte_out(REG_SCREEN_CTRL, 14);
    int offset = port_byte_in(REG_SCREEN_DATA) << 8;
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);
    return offset * 2;  /* Each cell is 2 bytes (char + attr) */
}

void set_cursor_offset(int offset) {
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}

/* ---- Coordinate utilities ---- */

int get_offset(int col, int row) {
    return 2 * (row * MAX_COLS + col);
}

int get_offset_row(int offset) {
    return offset / (2 * MAX_COLS);
}

int get_offset_col(int offset) {
    return (offset - (get_offset_row(offset) * 2 * MAX_COLS)) / 2;
}

/* ---- Scrolling ---- */

static int handle_scrolling(int cursor_offset) {
    if (cursor_offset < MAX_ROWS * MAX_COLS * 2) {
        return cursor_offset;
    }
    /* Scroll up: copy each row to the one above */
    unsigned char *vidmem = (unsigned char *) VIDEO_ADDRESS;
    for (int i = 1; i < MAX_ROWS; i++) {
        int dest = get_offset(0, i - 1);
        int src  = get_offset(0, i);
        for (int j = 0; j < MAX_COLS * 2; j++) {
            vidmem[dest + j] = vidmem[src + j];
        }
    }
    /* Clear the last row */
    int last_row = get_offset(0, MAX_ROWS - 1);
    for (int i = 0; i < MAX_COLS * 2; i += 2) {
        vidmem[last_row + i] = ' ';
        vidmem[last_row + i + 1] = WHITE_ON_BLACK;
    }
    cursor_offset -= 2 * MAX_COLS;
    return cursor_offset;
}
