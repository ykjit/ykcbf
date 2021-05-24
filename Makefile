all: bf_base

bf_base: bf_base.o
	clang -o bf_base bf_base.o

bf_base.o: bf_base.c
	clang -Wall -O3 -c bf_base.c

clean:
	rm -f bf_base bf_base.o
