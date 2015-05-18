#include "w_crt.h"

extern int main(int argc, char **argv);
void w_crt_exit(int error_code);

#define GET_EBP(ebp)			asm("movl %%ebp, %0":"=r"(ebp));
#define GET_ARGC(ebp, argc)		argc = *(int *)(ebp + 4);
#define GET_ARGV(ebp, argv)		argv = (char **)(ebp + 8);
	
void w_crt_entry(void)
{
	int ret, argc;
	char *ebp, **argv;
	
	GET_EBP(ebp)
	GET_ARGC(ebp, argc)
	GET_ARGV(ebp, argv)

	if (w_crt_heap_init() == -1)
		goto out;

	if (w_crt_io_init() == -1)
		goto out;

	ret = main(argc, argv);

out:
	w_crt_exit(ret);
}

void w_crt_exit(int error_code)
{
	asm("movl $1, %%eax\n\t"
	    "movl %0, %%ebx\n\t"
	    "int $0x80\n\t"
	    "hlt\n\t"::"m"(error_code));
}
