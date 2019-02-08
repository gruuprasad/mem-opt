#!/usr/bin/env bash
set -eu

make
clang -S -emit-llvm examples/lookup.c -oexamples/lookup.ll
opt -load lib/libTASPrototypePasses.so -loop-fission < examples/lookup.ll >/dev/null
