#include "w_crt.h"

int w_crt_io_init(void)
{
	return 0;
}

static int open(const char *path, int flags, int mode)
{
	int ret;

	asm("movl $5, %%eax\n\t"
		"movl %1, %%ebx\n\t"
		"movl %2, %%ecx\n\t"
		"movl %3, %%edx\n\t"
		"int $0x80\n\t"
		"movl %%eax, %0"
		:"=m"(ret):"m"(path),"m"(flags), "m"(mode));

	return ret;
}

static int read(int fd, void *buff, unsigned int size)
{
	int ret;
        asm("movl $3, %%eax\n\t"
                "movl %1, %%ebx\n\t"
                "movl %2, %%ecx\n\t"
                "movl %3, %%edx\n\t"
                "int $0x80\n\t"
                "movl %%eax, %0"
                :"=m"(ret):"m"(fd),"m"(buff), "m"(size));

        return ret;

}

static int write(int fd, void *buff, unsigned int size)
{
        int ret;
        asm("movl $4, %%eax\n\t"
                "movl %1, %%ebx\n\t"
                "movl %2, %%ecx\n\t"
                "movl %3, %%edx\n\t"
                "int $0x80\n\t"
                "movl %%eax, %0"
                :"=m"(ret):"m"(fd),"m"(buff), "m"(size));

        return ret;

}

static int seek(int fd, int offset, int mode)
{
	int ret;

	asm("movl $19, %%eax\n\t"
                "movl %1, %%ebx\n\t"
                "movl %2, %%ecx\n\t"
                "movl %3, %%edx\n\t"
                "int $0x80\n\t"
                "movl %%eax, %0"
                :"=m"(ret):"m"(fd),"m"(offset), "m"(mode));

        return ret;
}

static int close(int fd)
{
	int ret;

	asm("movl $6, %%eax\n\t"
		"int $0x80\n\t"
		"movl %%eax, %0"
		:"=m"(ret):"m"(fd));

	return ret;
}

FILE *fopen(char *path, char *mode)
{
	int fd, flags = 0;

	if (!strcmp(mode, "w"))
		flags |= O_WRONLY | O_CREAT | O_TRUNC;

	if (!strcmp(mode, "w+"))
		flags |= O_RDWR | O_CREAT | O_TRUNC;

	if (!strcmp(mode, "r"))
		flags |= O_RDONLY;

	if (!strcmp(mode, "r+"))
		flags |= O_RDWR | O_CREAT;
	
	fd = open(path, flags, 0700);

	return (FILE *)fd;
}

int fread(void *buffer, int size, int count, FILE *stream)
{
	return read((int)stream, buffer, size * count);
}

int fwrite(void *buffer, int size, int count, FILE *stream)
{
	return write((int)stream, buffer, size * count);
}

int fseek(FILE *stream, int offset, int set)
{
	return seek((int)stream, offset, set);
}

int fclose(FILE *stream)
{
	return close((int)stream);
}
