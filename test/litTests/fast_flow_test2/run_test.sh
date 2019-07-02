#!/usr/bin/env bash
set -eu

cur_dir=`pwd`

cd ../../../build
make
cd $cur_dir

# Run using debug build
LLVM_PATH=/home/gp/llvm_projects/llvm/install/bin

${LLVM_PATH}/opt -S -mem2reg  < fast_flows.ll > fast_flows_mem2reg.ll
${LLVM_PATH}/opt -S -load ../../../build/lib/libTASPass.so <fast_flows.ll > fast_flows_tas.ll -mem2reg -indvars -loop-simplify -lcssa -tas-batch-maker
${LLVM_PATH}/llc -filetype=obj fast_flows_tas.ll -o fast_flows.o
