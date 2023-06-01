#! /bin/sh

set -e

export CARGO_HOME="`pwd`/.cargo"
export RUSTUP_HOME="`pwd`/.rustup"
export RUSTUP_INIT_SKIP_PATH_CHECK="yes"
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs > rustup.sh
sh rustup.sh --default-host x86_64-unknown-linux-gnu \
    --default-toolchain nightly \
    --no-modify-path \
    --profile minimal \
    -y
export PATH=`pwd`/.cargo/bin/:$PATH

git clone --recurse-submodules --depth 1 https://github.com/softdevteam/yk
cd yk && cargo build
export PATH=`pwd`/bin:${PATH}
cd ..

# FIXME: We should test all optimisation levels.
# https://github.com/ykjit/ykcbf/issues/9
YK_BUILD_TYPE=debug CFLAGS=-O0 CFLAGS="-O0" make bf_base
YK_BUILD_TYPE=debug CFLAGS=-O0 make bf_simple_yk
YK_BUILD_TYPE=debug CFLAGS=-O0 make bf_simple2_yk

cd lang_tests && cargo test
