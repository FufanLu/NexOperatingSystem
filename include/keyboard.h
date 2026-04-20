#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

/* Read a line of input from keyboard into buffer */
void keyboard_read_line(char *buffer, int max_len);

/* Read a single scancode */
unsigned char keyboard_read_scancode(void);

#endif
