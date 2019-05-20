#!/usr/bin/env bash
set -eu

# Run using debug build
LLVM_PATH=/home/gp/llvm_projects/llvm/install/bin

# update input here
INPUT=lookup.c

#${LLVM_PATH}/clang -S -emit-llvm ../examples/$INPUT -ooutput.ll
#opt -load lib/libTASPrototypePasses.so < examples/lookup.ll >/dev/null -tas-loop-details
${LLVM_PATH}/opt -S -load lib/libTASPass.so <../examples/fast_flows_O0.ll > output.ll -tas-batch-process -debug-only=tas
