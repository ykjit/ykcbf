all:

bf_base: bf_base.o
	clang ${LDFLAGS} -o bf_base bf_base.o

bf_base.o: bf_base.c
	clang ${CFLAGS} -Wall -c bf_base.c

bf_simple_yk: bf_simple_yk.o
	clang ${LDFLAGS} -lykcapi -o bf_simple_yk bf_simple_yk.o

bf_simple_yk.o: bf_simple_yk.c
	clang ${CFLAGS} -Wall -c bf_simple_yk.c

bf_simple2_yk: bf_simple2_yk.o
	clang ${LDFLAGS} -lykcapi -o bf_simple2_yk bf_simple2_yk.o

bf_simple2_yk.o: bf_simple2_yk.c
	clang -Wall -O3 ${CFLAGS} -c bf_simple2_yk.c

clean:
	rm -f bf_base bf_base.o
