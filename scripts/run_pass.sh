#!/usr/bin/env bash
set -eu

make
# Run using debug build
LLVM_PATH=/home/gp/llvm_projects/llvm/install/bin

# update input here
INPUT=batch_process_test1.c

#${LLVM_PATH}/clang -S -emit-llvm $INPUT -ooutput.ll
#opt -load lib/libTASPrototypePasses.so < examples/lookup.ll >/dev/null -tas-loop-details
${LLVM_PATH}/opt -S -load lib/libTASPass.so <output.ll > output_tas.ll -mem2reg -indvars -loop-simplify -tas-batch-process -debug-only=tas
${LLVM_PATH}/llc -filetype=obj output_tas.ll -o batch_process_test1.o
${LLVM_PATH}/clang batch_process_test1.o -o batch_process_test1
