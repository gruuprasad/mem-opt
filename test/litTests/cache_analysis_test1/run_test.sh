#!/usr/bin/env bash
set -eu

cur_dir=`pwd`

cd ../../../build
make
cd $cur_dir

# Run using debug build
LLVM_PATH=/home/gp/llvm_projects/llvm/install/bin

# update input here
INPUT=cache_analysis_test1.c

${LLVM_PATH}/clang -S -emit-llvm -O1 -Xclang -disable-llvm-passes -c cache_analysis_test1.c
${LLVM_PATH}/opt -S -mem2reg  < cache_analysis_test1.ll > cache_analysis_test1_mem2reg.ll
${LLVM_PATH}/opt -S -load ../../../build/lib/libTASPass.so <cache_analysis_test1.ll > cache_analysis_test1_tas.ll -mem2reg -indvars -loop-simplify -lcssa -tas-cache-analysis
${LLVM_PATH}/llc -filetype=obj cache_analysis_test1_tas.ll -o cache_analysis_test1.o
${LLVM_PATH}/clang cache_analysis_test1.o -o cache_analysis_test1

# Run binary
./cache_analysis_test1

if [ $? -eq 0 ]; then
  echo TEST PASS
else
  echo TEST FAIL
fi
