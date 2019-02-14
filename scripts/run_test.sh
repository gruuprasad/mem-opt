#!/usr/bin/env bash
set -eu

make
clang -S -emit-llvm examples/lookup.c -oexamples/lookup.ll
#clang -S -emit-llvm examples/hello_world.c -oexamples/hello_world.ll
#opt -load lib/libTASPrototypePasses.so < examples/lookup.ll >/dev/null -tas-loop-details
opt -load lib/libTASPrototypePasses.so < examples/lookup.ll >/dev/null -tas-loop-fission
