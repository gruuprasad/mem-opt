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

/* Steps:
 *
 * 1. Identify batch and non-batch related arguments in scalar function (non-batch)
 * 2. Create batch function with appropriate input parameter types.
 * 3. Create Entry basic block [ENTRYBB].
 * 4. Move instructions with non-batch related arguments to entry basic block.
 * 5. Create basic block with set up related to batched argument, followed by entry basic block [BATCH_ENTRYBB].
 * 6. Add empty for loop followed by BATCH_ENTRYBB.
 * 7. Determine which basic blocks in scalar function is going to be part of the for loop body.
 * 8. Clone basic block into batch function.
 * 9. Replace scalar value uses with batched arguments.
 *    NOTE: First we form single loop, hence intermeditate values need not be batched.
 * 10. Move any remaining basic block followed by for loop if exists.
 */

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
