#!/usr/bin/env bash
set -eu

make
# Run using debug build
LLVM_PATH=/home/gp/llvm_projects/llvm/install/bin

${LLVM_PATH}/clang -S -emit-llvm examples/lookup.c -oexamples/lookup.ll
#clang -S -emit-llvm examples/hello_world.c -oexamples/hello_world.ll
#opt -load lib/libTASPrototypePasses.so < examples/lookup.ll >/dev/null -tas-loop-details
${LLVM_PATH}/opt -load lib/libTASPrototypePasses.so <examples/lookup.ll >/dev/null -tas-loop-fission -debug-only=tas
