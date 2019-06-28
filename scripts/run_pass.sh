#!/usr/bin/env bash
set -eu

make
# Run using debug build
LLVM_PATH=/home/gp/llvm_projects/llvm/install/bin
CUR_DIR=`pwd`

# update input here
INPUT=batch_process_test1.c

case "$1" in
  1) echo "Running fast_flows.ll"
#${LLVM_PATH}/clang -S -emit-llvm $INPUT -ooutput.ll
  ${LLVM_PATH}/opt -stats -S -load lib/libTASPass.so <fast_flows.ll > fast_flows_tas.ll -mem2reg -indvars -loop-simplify -tas-batch-process
  ${LLVM_PATH}/llc -filetype=obj fast_flows_tas.ll -o fast_flows.o
  ${LLVM_PATH}/llc fast_flows_tas.ll -o fast_flows.s
  ;;
#${LLVM_PATH}/clang -S -emit-llvm -c batch_process_test1.c
#${LLVM_PATH}/opt -S -load lib/libTASPass.so <batch_process_test1.ll > batch_process_test1_1.ll -tas-batch-process
#clang -Xclang -load -Xclang lib/libTASPass.so -O0 -c batch_process_test1.c
#${LLVM_PATH}/clang -Xclang -load -Xclang lib/libTASPass.so -O0 -S -emit-llvm batch_process_test1.c
  2) echo "Running batch_process_test1"
  ${LLVM_PATH}/clang -S -emit-llvm -O1 -Xclang -disable-llvm-passes -c batch_process_test1.c
  ${LLVM_PATH}/opt -S -mem2reg  < batch_process_test1.ll > batch_process_test1_mem2reg.ll
  ${LLVM_PATH}/opt -stats -S -load lib/libTASPass.so <batch_process_test1.ll > batch_process_test1_tas.ll -mem2reg -indvars -loop-simplify -lcssa -tas-batch-maker -tas-batch-process
  ${LLVM_PATH}/llc -filetype=obj batch_process_test1_tas.ll -o batch_process_test1.o
  ${LLVM_PATH}/clang batch_process_test1.o -o batch_process_test1
  ;;
# Unit test run
  3) echo "Running unit tests"
  ${CUR_DIR}/bin/tests --input_dir /home/gp/mpi-sws/tas-opt/test/unit/inputs/
  ;;
esac

#${LLVM_PATH}/opt -stats -S <fast_flows.ll > fast_flows_ref.ll -mem2reg
