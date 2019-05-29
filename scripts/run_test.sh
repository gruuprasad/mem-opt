#!/usr/bin/env bash
set -eu

make
# Run using debug build
LLVM_PATH=/home/gp/llvm_projects/llvm/install/bin

# update input here
#INPUT=lookup.c

#${LLVM_PATH}/clang -S -emit-llvm examples/$INPUT -oexamples/output.ll
#opt -load lib/libTASPrototypePasses.so < examples/lookup.ll >/dev/null -tas-loop-details
${LLVM_PATH}/opt -S -load lib/libTASPass.so <../examples/fast_flows.ll > output.ll -mem2reg -indvars -loop-simplify -lcssa -tas-batch-process -debug-only=tas
