#include "w_crt.h"

pid_t fork(void)
{
	int ret;

	asm("movl $2, %%eax\n\t"
		"int $0x80\n\t"
		"movl %%eax, %0"
		:"=r"(ret));
	
	return (pid_t)ret;
}

pid_t getpid(void)
{
        int ret;

        asm("movl $20, %%eax\n\t"
                "int $0x80\n\t"
                "movl %%eax, %0"
                :"=r"(ret));

        return (pid_t)ret;
}

pid_t getppid(void)
{
        int ret;

        asm("movl $64, %%eax\n\t"
                "int $0x80\n\t"
                "movl %%eax, %0"
                :"=r"(ret));

        return (pid_t)ret;
}

pid_t waitpid(pid_t pid, int *status, int options)
{
	int ret;

        asm("movl $7, %%eax\n\t"
		"movl %1, %%ebx\n\t"
		"movl %2, %%ecx\n\t"
		"movl %3, %%edx\n\t"
                "int $0x80\n\t"
                "movl %%eax, %0"
                :"=r"(ret):"m"(pid), "m"(status), "m"(options));

        return (pid_t)ret;
}

void exit(int status)
{
	asm("movl $1, %%eax\n\t"
		"movl %0, %%ebx\n\t"
		"int $0x80\n\t"
		::"m"(status));
}
