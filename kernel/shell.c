/* ============================================================
 * NexOS Shell
 * Command-line interface with built-in commands:
 *   help   - Show available commands
 *   list   - List files in the filesystem
 *   read   - Display file contents (cat)
 *   create - Create a new file with content
 *   delete - Delete a file
 *   rename - Rename a file
 *   copy   - Copy a file
 *   write  - Write content to a file
 *   clear  - Clear the screen
 *   info   - Show system information
 *   echo   - Print text to screen
 *   about  - About NexOS
 * ============================================================ */

#include "../include/shell.h"
#include "../include/screen.h"
#include "../include/keyboard.h"
#include "../include/filesystem.h"
#include "../include/string.h"

/* Forward declarations */
static void parse_command(char *input, char *cmd, char *arg1, char *arg2);
static void cmd_help(void);
static void cmd_list(void);
static void cmd_read(const char *filename);
static void cmd_create(const char *filename, const char *content);
static void cmd_delete(const char *filename);
static void cmd_rename(const char *old_name, const char *new_name);
static void cmd_copy(const char *src, const char *dest);
static void cmd_write(const char *filename, const char *content);
static void cmd_info(void);
static void cmd_echo(const char *text);
static void cmd_about(void);

/* ---- Shell Main Loop ---- */
void shell_run(void) {
    char input[MAX_CMD_LEN];
    char cmd[MAX_CMD_LEN];
    char arg1[MAX_CMD_LEN];
    char arg2[MAX_CMD_LEN];

    /* Print welcome banner */
    print_color("\n", WHITE_ON_BLACK);
    print_color("  =============================================\n", GREEN_ON_BLACK);
    print_color("       _   _           ___  ____\n", GREEN_ON_BLACK);
    print_color("      | \\ | |         / _ \\/ ___|\n", GREEN_ON_BLACK);
    print_color("      |  \\| | ___ _ _| | | \\___ \\\n", GREEN_ON_BLACK);
    print_color("      | |\\  |/ _ \\ \\ | |_| |___) |\n", GREEN_ON_BLACK);
    print_color("      |_| \\_|\\___/_\\_\\\\___/|____/\n", GREEN_ON_BLACK);
    print_color("\n", WHITE_ON_BLACK);
    print_color("       NexOS v1.0 - A Simple Operating System\n", GREEN_ON_BLACK);
    print_color("  =============================================\n", GREEN_ON_BLACK);
    print_color("\n  Type 'help' for a list of commands.\n\n", WHITE_ON_BLACK);

    while (1) {
        /* Print prompt */
        print_color("nexos", GREEN_ON_BLACK);
        print_color("> ", YELLOW_ON_BLACK);

        /* Read user input */
        keyboard_read_line(input, MAX_CMD_LEN);

        /* Skip empty input */
        if (strlen(input) == 0) continue;

        /* Parse command and arguments */
        parse_command(input, cmd, arg1, arg2);

        /* Execute command */
        if (strcmp(cmd, "help") == 0) {
            cmd_help();
        } else if (strcmp(cmd, "list") == 0 || strcmp(cmd, "ls") == 0) {
            cmd_list();
        } else if (strcmp(cmd, "read") == 0 || strcmp(cmd, "cat") == 0) {
            cmd_read(arg1);
        } else if (strcmp(cmd, "create") == 0 || strcmp(cmd, "touch") == 0) {
            cmd_create(arg1, arg2);
        } else if (strcmp(cmd, "delete") == 0 || strcmp(cmd, "rm") == 0) {
            cmd_delete(arg1);
        } else if (strcmp(cmd, "rename") == 0 || strcmp(cmd, "mv") == 0) {
            cmd_rename(arg1, arg2);
        } else if (strcmp(cmd, "copy") == 0 || strcmp(cmd, "cp") == 0) {
            cmd_copy(arg1, arg2);
        } else if (strcmp(cmd, "write") == 0) {
            cmd_write(arg1, arg2);
        } else if (strcmp(cmd, "clear") == 0 || strcmp(cmd, "cls") == 0) {
            clear_screen();
        } else if (strcmp(cmd, "info") == 0) {
            cmd_info();
        } else if (strcmp(cmd, "echo") == 0) {
            cmd_echo(arg1);
        } else if (strcmp(cmd, "about") == 0) {
            cmd_about();
        } else {
            print_color("  Error: Unknown command '", RED_ON_BLACK);
            print_color(cmd, RED_ON_BLACK);
            print_color("'\n", RED_ON_BLACK);
            print("  Type 'help' for available commands.\n");
        }
    }
}

/* ---- Command Parser ---- */
/* Splits "cmd arg1 arg2" into separate strings */
static void parse_command(char *input, char *cmd, char *arg1, char *arg2) {
    cmd[0] = '\0';
    arg1[0] = '\0';
    arg2[0] = '\0';

    int i = 0, j = 0;

    /* Skip leading spaces */
    while (input[i] == ' ') i++;

    /* Extract command */
    while (input[i] && input[i] != ' ') {
        cmd[j++] = input[i++];
    }
    cmd[j] = '\0';

    /* Skip spaces */
    while (input[i] == ' ') i++;

    /* Extract first argument */
    j = 0;
    if (input[i] == '"') {
        /* Quoted argument */
        i++;
        while (input[i] && input[i] != '"') {
            arg1[j++] = input[i++];
        }
        if (input[i] == '"') i++;
    } else {
        while (input[i] && input[i] != ' ') {
            arg1[j++] = input[i++];
        }
    }
    arg1[j] = '\0';

    /* Skip spaces */
    while (input[i] == ' ') i++;

    /* Extract second argument (rest of line) */
    j = 0;
    if (input[i] == '"') {
        i++;
        while (input[i] && input[i] != '"') {
            arg2[j++] = input[i++];
        }
    } else {
        while (input[i]) {
            arg2[j++] = input[i++];
        }
    }
    arg2[j] = '\0';
}

/* ---- Command Implementations ---- */

static void cmd_help(void) {
    print("\n");
    print_color("  === NexOS Command Reference ===\n\n", CYAN_ON_BLACK);
    print_color("  File Commands:\n", YELLOW_ON_BLACK);
    print("    list  (ls)     - List all files\n");
    print("    read  (cat)    - Read file contents       : read <filename>\n");
    print("    create (touch) - Create a new file        : create <name> <content>\n");
    print("    delete (rm)    - Delete a file             : delete <filename>\n");
    print("    rename (mv)    - Rename a file             : rename <old> <new>\n");
    print("    copy  (cp)     - Copy a file               : copy <src> <dest>\n");
    print("    write          - Write content to file     : write <name> <content>\n");
    print_color("\n  System Commands:\n", YELLOW_ON_BLACK);
    print("    clear (cls)    - Clear the screen\n");
    print("    info           - Show system information\n");
    print("    echo           - Print text to screen      : echo <text>\n");
    print("    about          - About NexOS\n");
    print("    help           - Show this help message\n");
    print("\n");
}

static void cmd_list(void) {
    print("\n");
    fs_list_files();
    print("\n");
}

static void cmd_read(const char *filename) {
    if (strlen(filename) == 0) {
        print_color("  Usage: read <filename>\n", RED_ON_BLACK);
        return;
    }
    char buffer[MAX_FILE_SIZE];
    int result = fs_read_file(filename, buffer);
    if (result < 0) {
        print_color("  Error: File '", RED_ON_BLACK);
        print_color(filename, RED_ON_BLACK);
        print_color("' not found.\n", RED_ON_BLACK);
        return;
    }
    print("\n  --- ");
    print_color(filename, CYAN_ON_BLACK);
    print(" ---\n  ");
    /* Print content with indentation */
    for (int i = 0; buffer[i]; i++) {
        print_char(buffer[i], -1, -1, WHITE_ON_BLACK);
        if (buffer[i] == '\n' && buffer[i+1]) {
            print("  ");
        }
    }
    print("\n\n");
}

static void cmd_create(const char *filename, const char *content) {
    if (strlen(filename) == 0) {
        print_color("  Usage: create <filename> [content]\n", RED_ON_BLACK);
        return;
    }
    const char *file_content = strlen(content) > 0 ? content : "";
    int result = fs_create_file(filename, file_content);
    if (result == -1) {
        print_color("  Error: File already exists.\n", RED_ON_BLACK);
    } else if (result == -2) {
        print_color("  Error: Filesystem full.\n", RED_ON_BLACK);
    } else {
        print_color("  File '", GREEN_ON_BLACK);
        print_color(filename, GREEN_ON_BLACK);
        print_color("' created.\n", GREEN_ON_BLACK);
    }
}

static void cmd_delete(const char *filename) {
    if (strlen(filename) == 0) {
        print_color("  Usage: delete <filename>\n", RED_ON_BLACK);
        return;
    }
    int result = fs_delete_file(filename);
    if (result < 0) {
        print_color("  Error: File '", RED_ON_BLACK);
        print_color(filename, RED_ON_BLACK);
        print_color("' not found.\n", RED_ON_BLACK);
    } else {
        print_color("  File '", GREEN_ON_BLACK);
        print_color(filename, GREEN_ON_BLACK);
        print_color("' deleted.\n", GREEN_ON_BLACK);
    }
}

static void cmd_rename(const char *old_name, const char *new_name) {
    if (strlen(old_name) == 0 || strlen(new_name) == 0) {
        print_color("  Usage: rename <old_name> <new_name>\n", RED_ON_BLACK);
        return;
    }
    int result = fs_rename_file(old_name, new_name);
    if (result == -1) {
        print_color("  Error: Source file not found.\n", RED_ON_BLACK);
    } else if (result == -2) {
        print_color("  Error: Destination name already exists.\n", RED_ON_BLACK);
    } else {
        print_color("  Renamed '", GREEN_ON_BLACK);
        print_color(old_name, GREEN_ON_BLACK);
        print_color("' -> '", GREEN_ON_BLACK);
        print_color(new_name, GREEN_ON_BLACK);
        print_color("'\n", GREEN_ON_BLACK);
    }
}

static void cmd_copy(const char *src, const char *dest) {
    if (strlen(src) == 0 || strlen(dest) == 0) {
        print_color("  Usage: copy <source> <destination>\n", RED_ON_BLACK);
        return;
    }
    int result = fs_copy_file(src, dest);
    if (result == -1) {
        print_color("  Error: Source file not found.\n", RED_ON_BLACK);
    } else if (result == -2) {
        print_color("  Error: Destination already exists.\n", RED_ON_BLACK);
    } else if (result == -3) {
        print_color("  Error: Filesystem full.\n", RED_ON_BLACK);
    } else {
        print_color("  Copied '", GREEN_ON_BLACK);
        print_color(src, GREEN_ON_BLACK);
        print_color("' -> '", GREEN_ON_BLACK);
        print_color(dest, GREEN_ON_BLACK);
        print_color("'\n", GREEN_ON_BLACK);
    }
}

static void cmd_write(const char *filename, const char *content) {
    if (strlen(filename) == 0 || strlen(content) == 0) {
        print_color("  Usage: write <filename> <content>\n", RED_ON_BLACK);
        return;
    }
    int result = fs_write_file(filename, content);
    if (result < 0) {
        print_color("  Error: Could not write to file.\n", RED_ON_BLACK);
    } else {
        print_color("  Written to '", GREEN_ON_BLACK);
        print_color(filename, GREEN_ON_BLACK);
        print_color("'\n", GREEN_ON_BLACK);
    }
}

static void cmd_info(void) {
    char num_buf[12];
    print("\n");
    print_color("  === System Information ===\n\n", CYAN_ON_BLACK);
    print("    OS Name       : NexOS\n");
    print("    Version       : 1.0\n");
    print("    Architecture  : x86 (32-bit Protected Mode)\n");
    print("    Video Mode    : VGA Text Mode (80x25)\n");
    print("    Filesystem    : NexFS (In-Memory)\n");
    print("    Max Files     : 32\n");
    print("    Files Used    : ");
    itoa(fs_get_file_count(), num_buf);
    print(num_buf);
    print("\n");
    print("    Author        : Fufan Lu\n");
    print("    Built for     : NEU MSIS OS Course\n");
    print("\n");
}

static void cmd_echo(const char *text) {
    if (strlen(text) == 0) {
        print("\n");
    } else {
        print("  ");
        print(text);
        print("\n");
    }
}

static void cmd_about(void) {
    print("\n");
    print_color("  =============================================\n", GREEN_ON_BLACK);
    print_color("  NexOS - A Simple Operating System\n", GREEN_ON_BLACK);
    print_color("  =============================================\n", GREEN_ON_BLACK);
    print("\n");
    print("  NexOS is a minimal operating system built from\n");
    print("  scratch using x86 Assembly and C. It features:\n\n");
    print("    - Custom bootloader (16-bit Real Mode)\n");
    print("    - Protected Mode (32-bit) transition\n");
    print("    - VGA text-mode screen driver\n");
    print("    - PS/2 keyboard driver (polling)\n");
    print("    - In-memory filesystem (NexFS)\n");
    print("    - Interactive command-line shell\n");
    print("\n");
    print("  Developed as a course project for\n");
    print("  Northeastern University, Seattle Campus.\n");
    print("\n");
}
