/* ============================================================
 * Simple In-Memory Filesystem
 * Flat file system with create, delete, rename, copy,
 * read, write, and list operations.
 * ============================================================ */

#include "../include/filesystem.h"
#include "../include/string.h"
#include "../include/screen.h"

/* Global file table */
static FileEntry file_table[MAX_FILES];

/* Initialize filesystem with some default files */
void fs_init(void) {
    memset(file_table, 0, sizeof(file_table));

    /* Create some default files */
    fs_create_file("readme.txt", "Welcome to NexOS!\nA simple operating system built from scratch.\nType 'help' for available commands.");
    fs_create_file("hello.txt", "Hello, World!\nThis is a sample text file.");
    fs_create_file("system.log", "[BOOT] NexOS v1.0 started\n[INIT] Filesystem initialized\n[INIT] Shell ready");
    fs_create_file("notes.txt", "TODO:\n- Add more commands\n- Implement interrupts\n- Add memory management");
}

/* Find file index by name, returns -1 if not found */
static int find_file(const char *name) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (file_table[i].is_used && strcmp(file_table[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

/* Find a free slot, returns -1 if full */
static int find_free_slot(void) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (!file_table[i].is_used) return i;
    }
    return -1;
}

/* Create a new file with optional content */
int fs_create_file(const char *name, const char *content) {
    if (find_file(name) >= 0) return -1;  /* Already exists */

    int idx = find_free_slot();
    if (idx < 0) return -2;  /* No space */

    strcpy(file_table[idx].name, name);
    if (content) {
        strcpy(file_table[idx].content, content);
        file_table[idx].size = strlen(content);
    } else {
        file_table[idx].content[0] = '\0';
        file_table[idx].size = 0;
    }
    file_table[idx].is_used = 1;
    file_table[idx].is_directory = 0;
    file_table[idx].parent_idx = -1;

    return 0;
}

/* Delete a file by name */
int fs_delete_file(const char *name) {
    int idx = find_file(name);
    if (idx < 0) return -1;  /* Not found */

    file_table[idx].is_used = 0;
    file_table[idx].name[0] = '\0';
    file_table[idx].content[0] = '\0';
    file_table[idx].size = 0;
    return 0;
}

/* Rename a file */
int fs_rename_file(const char *old_name, const char *new_name) {
    int idx = find_file(old_name);
    if (idx < 0) return -1;  /* Source not found */
    if (find_file(new_name) >= 0) return -2;  /* Dest already exists */

    strcpy(file_table[idx].name, new_name);
    return 0;
}

/* Copy a file */
int fs_copy_file(const char *src, const char *dest) {
    int src_idx = find_file(src);
    if (src_idx < 0) return -1;  /* Source not found */
    if (find_file(dest) >= 0) return -2;  /* Dest already exists */

    int dest_idx = find_free_slot();
    if (dest_idx < 0) return -3;  /* No space */

    strcpy(file_table[dest_idx].name, dest);
    strcpy(file_table[dest_idx].content, file_table[src_idx].content);
    file_table[dest_idx].size = file_table[src_idx].size;
    file_table[dest_idx].is_used = 1;
    file_table[dest_idx].is_directory = 0;
    file_table[dest_idx].parent_idx = -1;

    return 0;
}

/* Read file content into buffer */
int fs_read_file(const char *name, char *buffer) {
    int idx = find_file(name);
    if (idx < 0) return -1;

    strcpy(buffer, file_table[idx].content);
    return file_table[idx].size;
}

/* Write/overwrite content to a file */
int fs_write_file(const char *name, const char *content) {
    int idx = find_file(name);
    if (idx < 0) {
        /* File doesn't exist, create it */
        return fs_create_file(name, content);
    }
    strcpy(file_table[idx].content, content);
    file_table[idx].size = strlen(content);
    return 0;
}

/* List all files with sizes */
void fs_list_files(void) {
    char num_buf[12];
    int count = 0;

    print_color("  NAME                          SIZE (bytes)\n", CYAN_ON_BLACK);
    print_color("  ----                          ------------\n", CYAN_ON_BLACK);

    for (int i = 0; i < MAX_FILES; i++) {
        if (file_table[i].is_used) {
            print("  ");
            print(file_table[i].name);

            /* Pad to align size column */
            int name_len = strlen(file_table[i].name);
            for (int j = name_len; j < 30; j++) {
                print(" ");
            }

            itoa(file_table[i].size, num_buf);
            print(num_buf);
            print("\n");
            count++;
        }
    }

    print("\n  Total files: ");
    itoa(count, num_buf);
    print(num_buf);
    print("\n");
}

/* Check if a file exists */
int fs_file_exists(const char *name) {
    return find_file(name) >= 0;
}

/* Get total number of files */
int fs_get_file_count(void) {
    int count = 0;
    for (int i = 0; i < MAX_FILES; i++) {
        if (file_table[i].is_used) count++;
    }
    return count;
}

/* Get file size by name */
int fs_get_file_size(const char *name) {
    int idx = find_file(name);
    if (idx < 0) return -1;
    return file_table[idx].size;
}
