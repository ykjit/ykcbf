CFLAGS ?= -O3

bf: bf.o
	${CC} ${LDFLAGS} -o bf bf.o

bf.o: bf.c
	${CC} ${CFLAGS} -Wall -c bf.c

clean:
	rm -f bf bf.o
