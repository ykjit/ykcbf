# The clang used should be from ykllvm.
CC = clang
CFLAGS += -Wall

# Flags required for the JIT to work. The user is expected to set the YK_DIR
# environment to their (compiled) `yk` directory.
#
# See `c_tests/run.rs` in the `yk` repo for the meaning of all of these flags.
YK_CFLAGS =	-I${YK_DIR}/ykcapi \
		-flto
YK_LDFLAGS =	-L${YK_DIR}/target/debug/deps \
		-Wl,-rpath=${YK_DIR}/target/debug/deps \
		-flto \
		-fuse-ld=lld \
		-lykcapi \
		-Wl,--mllvm=--embed-bitcode-final \
		-Wl,--lto-basic-block-sections=labels \
		-Wl,--mllvm=--disable-branch-fold \
		-Wl,--mllvm=--disable-block-placement \
		-Wl,--mllvm=--disable-early-taildup \
		-Wl,--mllvm=--disable-tail-duplicate \
		-Wl,--mllvm=--yk-patch-control-point \
        	-Wl,--mllvm=--yk-block-disambiguate \
		-Wl,--lto-basic-block-sections=labels \
		-Wl,--export-dynamic

all: bf_base bf_simple_yk bf_simple2_yk

bf_base: bf_base.o
	clang ${LDFLAGS} -o $@ $<

bf_simple_yk.o: bf_simple_yk.c
	${CC} ${CFLAGS} ${YK_CFLAGS} -c -o $@ $<

bf_simple_yk: bf_simple_yk.o
	clang ${YK_LDFLAGS} ${LDFLAGS} -o $@ $<

bf_simple2_yk.o: bf_simple2_yk.c
	${CC} ${CFLAGS} ${YK_CFLAGS} -c -o $@ $<

bf_simple2_yk: bf_simple2_yk.o
	clang ${YK_LDFLAGS} ${LDFLAGS} -o $@ $<

clean:
	rm -f bf_base bf_base.o \
		bf_simple_yk bf_simple_yk.o \
		bf_simple2_yk bf_simple2_yk.o
