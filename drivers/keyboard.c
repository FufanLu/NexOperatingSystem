/* ============================================================
 * Keyboard Driver
 * Reads scancodes from the PS/2 keyboard controller and
 * converts them to ASCII characters.
 * Uses polling (no interrupts) for simplicity.
 * ============================================================ */

#include "../include/keyboard.h"
#include "../include/ports.h"
#include "../include/screen.h"

/* US QWERTY scancode-to-ASCII lookup table (set 1) */
static const char scancode_ascii[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=','\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0,  'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,  '\\','z','x','c','v','b','n','m',',','.','/',0,
    '*', 0, ' '
};

/* Read a single scancode from the keyboard data port */
unsigned char keyboard_read_scancode(void) {
    unsigned char status;
    /* Wait until the keyboard buffer has data */
    do {
        status = port_byte_in(KEYBOARD_STATUS_PORT);
    } while (!(status & 0x01));
    return port_byte_in(KEYBOARD_DATA_PORT);
}

/* Read a full line from the keyboard (blocking) */
void keyboard_read_line(char *buffer, int max_len) {
    int pos = 0;
    while (1) {
        unsigned char scancode = keyboard_read_scancode();

        /* Ignore key releases (bit 7 set) */
        if (scancode & 0x80) continue;

        /* Handle Enter key */
        if (scancode_ascii[scancode] == '\n') {
            buffer[pos] = '\0';
            print("\n");
            return;
        }

        /* Handle Backspace */
        if (scancode_ascii[scancode] == '\b') {
            if (pos > 0) {
                pos--;
                print_char('\b', -1, -1, WHITE_ON_BLACK);
            }
            continue;
        }

        /* Regular character */
        char c = scancode_ascii[scancode];
        if (c != 0 && pos < max_len - 1) {
            buffer[pos++] = c;
            print_char(c, -1, -1, WHITE_ON_BLACK);
        }
    }
}
