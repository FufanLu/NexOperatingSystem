#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#define MAX_FILES       32
#define MAX_FILENAME    32
#define MAX_FILE_SIZE   512
#define MAX_PATH        64

/* File entry structure */
typedef struct {
    char   name[MAX_FILENAME];
    char   content[MAX_FILE_SIZE];
    int    size;          /* Content length */
    int    is_used;       /* 1 = occupied, 0 = free */
    int    is_directory;  /* 1 = directory, 0 = regular file */
    int    parent_idx;    /* Index of parent directory, -1 for root */
} FileEntry;

/* Filesystem API */
void  fs_init(void);
int   fs_create_file(const char *name, const char *content);
int   fs_delete_file(const char *name);
int   fs_rename_file(const char *old_name, const char *new_name);
int   fs_copy_file(const char *src, const char *dest);
int   fs_read_file(const char *name, char *buffer);
int   fs_write_file(const char *name, const char *content);
void  fs_list_files(void);
int   fs_file_exists(const char *name);
int   fs_get_file_count(void);
int   fs_get_file_size(const char *name);

#endif
