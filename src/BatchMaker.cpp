#include "BatchMaker.h"
#include "Util.h"

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/Statistic.h>
#include <llvm/IR/Argument.h>
#include <llvm/IR/CFG.h>
#include <llvm/IR/IRBuilder.h>

#include <iostream>
#include <string>

using namespace llvm;

#define DEBUG_TYPE "tas-batch-maker"

namespace tas {

bool BatchMaker::run() {

  createBatchedFormFn();
  OldFunc->getParent()->dump();

  return true;
}

SmallVector<Argument *, 4> BatchMaker::getBatchArgs() {
  SmallVector<Argument *, 4> BatchParams;
  for (auto & A : OldFunc->args())
      BatchParams.push_back(&A);
  return BatchParams;
}

void BatchMaker::createBatchedFormFn() {
  auto BatchParams = getBatchArgs();

  // Create batch parameters
  SmallVector<Type *, 4> NewArgs;
  for (auto & Param : BatchParams) {
    NewArgs.push_back(PointerType::get(Param->getType(), 0));
    errs() << "New type = " << *NewArgs.back() << "\n";
  }

  // Create Function prototype
  auto RetType = OldFunc->getReturnType();
  FunctionType *BatchFuncType = FunctionType::get(RetType, NewArgs, false);

  NewFunc = Function::Create(BatchFuncType, GlobalValue::ExternalLinkage,
                                        "batch_fn", OldFunc->getParent());

  BasicBlock *ReturnBlock = BasicBlock::Create(OldFunc->getContext(), "RET_BLOCK", NewFunc, 0);
  ReturnInst::Create(NewFunc->getContext(), Constant::getNullValue(RetType), ReturnBlock);
}

} // tas namespace
