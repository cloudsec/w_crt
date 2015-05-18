#include "w_crt.h"

#define va_list				char*
#define va_start(arg, fortmat)		(arg = (char *)&format + sizeof(format))
#define va_arg(arg, format)		(*(format *)((arg += sizeof(format)) - sizeof(format)))
#define va_end(arg)			*(char *)arg = 0

int fputc(int c, FILE *stream)
{
	return fwrite(&c, 1, 1, stream);
}

int fputs(char *str, FILE *stream)
{
	int len = strlen(str);

	return fwrite(str, 1, len, stream);
}

int vfprintf(FILE *fp, char *format, va_list arg)
{
	int flag = 0, ret = 0;
	const char *p = format;

	while (*p) {
		switch (*p) {
		case '%':
			if (flag) {
				flag = 0;
				if (fputc(*p, fp) < 0)
					return EOF;
				ret++;
			}
			else {
				flag = 1;
			}
			break;
		case 'd':
			if (flag) {
				char buf[16];
				flag = 0;

				itoa(va_arg(arg, int), buf, 10);
				if (fputs(buf, fp) < 0)
					return EOF;
				ret += strlen(buf);
			}
			else {
				if (fputc(*p, fp) < 0)
					return EOF;
				ret++;
			}
			break;
		case 's':
			if (flag) {
				const char *str = va_arg(arg, char*);
				flag = 0;
				
				if (fputs(str, fp) < 0)
					return EOF;
				ret += strlen(str);
			}
			else {
				if (fputc(*p, fp) < 0)
					return EOF;
				ret++;
			}
			break;
                case 'c':
                        if (flag) {
                                const char s = va_arg(arg, char);
                                flag = 0;

                                if (fputc(s, fp) < 0)
                                        return EOF;
				ret++;
                        }
                        else {
                                if (fputc(*p, fp) < 0)
                                        return EOF;
                                ret++;
                        }
                        break;
		default:
			if (fputc(*p, fp) < 0)
				return EOF;
			else
				ret++;
			break;
		}
		*p++;
	}

	va_end(arg);
	return ret;
}

int printf(const char *format, ...)
{
	va_list arg;
	va_start(arg, format);

	return vfprintf(stdout, format, arg);
}

/*
int main(void)
{
	char *s = "sadfafad";
	char c = 'A';
	int a = 4;

	printf("test is %swwwwwww", s);
	printf("test is %d + %d = %d", a, 3, a + 3);
	printf("test %c", c);
	return 0;
}
*/
