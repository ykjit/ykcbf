CFLAGS += -Wall

# Flags required for the JIT to work.
#
# The user is expected to have:
#  - put `yk-config` in the `PATH`
#  - set `YK_BUILD_TYPE` to either `debug` or `release`.
CC=`yk-config ${YK_BUILD_TYPE} --cc`
YK_CFLAGS=`yk-config ${YK_BUILD_TYPE} --cflags --cppflags`
YK_LDFLAGS=`yk-config ${YK_BUILD_TYPE} --ldflags --libs`

all: bf_base bf_simple_yk bf_simple2_yk

bf_base: bf_base.o
	${CC} ${LDFLAGS} -o $@ $<

bf_simple_yk.o: bf_simple_yk.c
	${CC} ${CFLAGS} ${YK_CFLAGS} -c -o $@ $<

bf_simple_yk: bf_simple_yk.o
	${CC} ${YK_LDFLAGS} ${LDFLAGS} -o $@ $<

bf_simple2_yk.o: bf_simple2_yk.c
	${CC} ${CFLAGS} ${YK_CFLAGS} -c -o $@ $<

bf_simple2_yk: bf_simple2_yk.o
	${CC} ${YK_LDFLAGS} ${LDFLAGS} -o $@ $<

.PHONY: test
test: all
	cd lang_tests && cargo test

clean:
	rm -f bf_base bf_base.o \
		bf_simple_yk bf_simple_yk.o \
		bf_simple2_yk bf_simple2_yk.o
