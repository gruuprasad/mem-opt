#!/usr/bin/env bash
set -eu

cur_dir=`pwd`

cd ../../../build
make
cd $cur_dir

# Run using debug build
LLVM_PATH=/home/gp/llvm_projects/llvm/install/bin

# update input here
INPUT=batch_process_test1.c

${LLVM_PATH}/clang -S -emit-llvm -O1 -Xclang -disable-llvm-passes -c batch_process_test1.c
${LLVM_PATH}/opt -S -mem2reg  < batch_process_test1.ll > batch_process_test1_mem2reg.ll
${LLVM_PATH}/opt -S -load ../../../build/lib/libTASPass.so <batch_process_test1.ll > batch_process_test1_tas.ll -mem2reg -indvars -loop-simplify -lcssa -tas-batch-maker -tas-batch-process
${LLVM_PATH}/llc -filetype=obj batch_process_test1_tas.ll -o batch_process_test1.o
${LLVM_PATH}/clang -c test.c -o test1.o
${LLVM_PATH}/clang batch_process_test1.o test1.o -o batch_process_test1

# Run binary
./batch_process_test1

if [ $? -eq 0 ]; then
  echo TEST PASS
else
  echo TEST FAIL
fi
