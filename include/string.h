#ifndef STRING_H
#define STRING_H

int   strcmp(const char *s1, const char *s2);
int   strncmp(const char *s1, const char *s2, int n);
int   strlen(const char *s);
void  strcpy(char *dest, const char *src);
void  strcat(char *dest, const char *src);
char *strchr(const char *s, char c);
void  itoa(int value, char *buffer);
void  memcpy(void *dest, const void *src, int n);
void  memset(void *dest, unsigned char val, int n);

#endif
