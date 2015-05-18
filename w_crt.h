#ifndef W_CRT_H
#define W_CRT_H

#define NULL		(void *)0

void *malloc(unsigned int size);
void free(void *addr);

typedef int FILE;

#define EOF		(-1)
#define stdin		((FILE *)0)
#define stdout		((FILE *)1)
#define stderr		((FILE *)2)

#define O_ACCMODE          0003
#define O_RDONLY             00
#define O_WRONLY             01
#define O_RDWR               02
#define O_CREAT            0100 /* not fcntl */
#define O_EXCL             0200 /* not fcntl */
#define O_NOCTTY           0400 /* not fcntl */
#define O_TRUNC           01000 /* not fcntl */
#define O_APPEND          02000
#define O_NONBLOCK        04000
#define O_NDELAY        O_NONBLOCK
#define O_SYNC         04010000
#define O_FSYNC          O_SYNC
#define O_ASYNC          020000

FILE *fopen(char *path, char *mode);
int fread(void *buffer, int size, int count, FILE *stream);
int fwrite(void *buffer, int size, int count, FILE *stream);
int fseek(FILE *stream, int offset, int set);
int fclose(FILE *stream);
int fputc(int c, FILE *stream);
int fputs(char *str, FILE *stream);
int printf(const char *format, ...);

char *itoa(int num, char *str, int radix);
int strlen(char *str);
char *strcpy(char *dst, char *src);
char *strncpy(char *dst, char *src, int n);
int strcmp(char *src, char *dst);
int strncmp(char *src, char *dst, int n);
void *memcpy(void *dst, void *src, int n);
void *memset(void *s, int c, int n);

typedef unsigned int pid_t;
pid_t fork(void);
pid_t getpid(void);
pid_t getppid(void);
pid_t waitpid(pid_t pid, int *status, int options);
void exit(int status);

#endif
