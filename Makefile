all: w_crt.a test

w_crt.a:
	gcc -c -fno-builtin -nostdlib -fno-stack-protector entry.c fork.c malloc.c printf.c stdio.c string.c
	ar -rs w_crt.a malloc.o printf.o stdio.o string.o fork.o

test:
	gcc -c -fno-builtin -nostdlib -fno-stack-protector test.c
	ld -static -e w_crt_entry entry.o test.o w_crt.a -o test

clean:
	rm -f test *.o *.a
