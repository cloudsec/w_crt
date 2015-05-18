#include "w_crt.h"

char *itoa(int num, char *str, int radix)
{
        char string[] = "0123456789abcdefghijklmnopqrstuvwxyz";
        char *s = str, *p;
        char tmp;

        if (num < 0)
                return NULL;

        if (!num) {
                *s++ = '\0';
                *s = 0;
                return s;
        }

        while (num) {
                *s++ = string[num % radix];
                num /= radix;
        }
        *s = 0;

        --s;
        for (p = str; p < s; s--, p++) {
                tmp = *p;
                *p = *s;
                *s = tmp;
        }

        return s;
}

int strlen(char *str)
{
	char *s = str;
	
	if (!s)
		return -1;

	while (*s++);

	return (int)(s - str - 1);
}

char *strcpy(char *dst, char *src)
{
	char *s = dst, *p = src;

	while (*p)
		*s++ = *p++;
	*s = 0;

	return dst;
}

char *strncpy(char *dst, char *src, int n)
{
	char *s = src, *p = dst;
	int i = 0;

	while (*s && i < n) {
		*p++ = *s++;
		i++;
	}
	*p = 0;

	return dst;
}

char *_strcat(char *dst, char *src)
{
	char *s = dst, *p = src;

	while (*s++);

	s--;
	while (*p)
		*s++ = *p++;
	*s = 0;

	return dst;
}

char *_strncat(char *dst, char *src, int n)
{
	char *s = dst, *p = src;
	int i = 0;

	while (*s++);

	s--;
	while (*p && (i++ < n))
		*s++ = *p++;
	*s = 0;

	return dst;
}

char *_strchr(char *src, int c)
{
	char *s = src;

	if (!s)
		return NULL;

	while (*s && *s != c) s++;

	return s;
}

char *_strrchr(char *src, int c)
{
	char *s = src;
	int i = 0;

	if (!s)
		return NULL;

	while (*s++) i++;

	s -= 2; i -= 1;
	while (i-- >= 0) {
		if (*s == c)
			return s;
		s--;
	}

	return NULL;
}

int strcmp(char *src, char *dst)
{
	char *s = src, *p = dst;

	while (*s && *p) {
		if (*s++ != *p++)
			break;
	}

	if (*s == *p)
		return 0;
	if (*s > *p)
		return 1;
	if (*s < *p)
		return -1;
}

int _strncmp(char *src, char *dst, int n)
{
        char *s = src, *p = dst;
	int flag = 0;
	int i = 0;

        while (*s && *p && i++ < n) {
		if (*s++ != *p++) {
			flag = 1;
			break;
		}
        }

	if (flag) {
		if (*s == *p)
			return 0;
		if (*s > *p)
			return 1;
		if (*s < *p)
			return -1;
	}
	else
		return 0;
}

void *memcpy(void *dst, void *src, int n)
{
        char *s = src, *p = dst;
        int i = 0;

        while (i < n) {
                *p++ = *s++;
                i++;
        }
        *p = 0;
}

void *memset(void *s, int c, int n)
{
	int i;
	char *p = s;

	for (i = 0; i < n; i++)
		*p++ = c;
}

/*
int main(void)
{
	char *s = "abcd";
	char buff[128] = "wcaaaabbbbzhitongcccddd";
	char *p;
	
	printf("%d\n", strlen(s));
	printf("%d\n", strcmp("aba", "aa"));
	printf("%s\n", _strcat(buff, s));
	printf("%s\n", _strncat(buff, s, 1));
	printf("%s\n", _strrchr(buff, 'c'));

	if (!_strncmp(buff, "bb", 3))
		printf("!\n");
	else
		printf("!!\n");
}
*/
