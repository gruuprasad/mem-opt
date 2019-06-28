#include "BatchMaker.h"
#include "ForLoop.h"
#include "Util.h"

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/Statistic.h>
#include <llvm/IR/Argument.h>
#include <llvm/IR/CFG.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Transforms/Utils/Cloning.h>

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

void BatchMaker::createBatchedFormFn() {
  SmallVector<Value *, 4> BatchArgs;
  detectBatchingParameters(OldFunc, BatchArgs);

  // Create batch parameters
  SmallVector<Type *, 4> NewArgs;
  for (auto & Param : BatchArgs) {
    NewArgs.push_back(PointerType::get(Param->getType(), 0));
    errs() << "New type = " << *NewArgs.back() << "\n";
  }

  // Create Function prototype
  auto RetType = OldFunc->getReturnType();
  FunctionType *BatchFuncType = FunctionType::get(RetType, NewArgs, false);
  NewFunc = Function::Create(BatchFuncType, GlobalValue::ExternalLinkage,
                                        "batch_fn", OldFunc->getParent());

  // Create empty entry basic block
  auto * EntryBB = BasicBlock::Create(NewFunc->getContext(), "entry", NewFunc);
  IRBuilder<> Builder(EntryBB);
  Builder.CreateRet(Constant::getNullValue(RetType));

  // For each batch argument, store value on stack using alloca.
  Builder.SetInsertPoint(EntryBB, EntryBB->begin());
  for (auto & A : NewFunc->args()) {
    auto APtr = Builder.CreateAlloca(A.getType());
    Builder.CreateStore(&A, APtr);
  }

  /*
  auto NewArgIt = NewFunc->arg_begin();
  for (auto & A : OldFunc->args()) {
    auto & NewValue = *NewArgIt++;
    errs() << "Old Argument = " << A << "New argument = " << NewValue << "\n";
    replaceUsesWithinBB(&A, &NewValue, EntryBB);
  }
  */

  //auto TL0 = TASForLoop(NewFunc->getContext(), , ReturnBlock, "tas.loop." + std::to_string(i), F);
}

} // tas namespace
