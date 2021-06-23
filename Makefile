all:

bf_replay_jit: bf_replay_jit.c
	@if [ "${YK_DIR}" = "" ]; then \
		echo "Please set YK_DIR in the environment"; \
		exit 1; \
		fi
	clang ${CFLAGS} ${LDFLAGS} \
		-L${YK_DIR}/target/debug/deps \
		-Wl,-rpath=${YK_DIR}/target/debug/deps \
		-lykcapi \
		-I${YK_DIR}/ykcapi \
		-fuse-ld=lld \
		-flto \
		-Wl,--mllvm=--embed-bitcode-final \
		-Wl,--lto-basic-block-sections=labels \
		-o $@ bf_replay_jit.c

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
	rm -f bf_base bf_base.o bf_replay_jit
