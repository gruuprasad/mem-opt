#include "BatchMaker.h"
#include "BatchProcess.h"
#include "ForLoop.h"
#include "Util.h"

#include <llvm/ADT/PostOrderIterator.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/Statistic.h>
#include <llvm/IR/Argument.h>
#include <llvm/IR/CFG.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/ValueSymbolTable.h>
#include <llvm/Transforms/Utils/BasicBlockUtils.h>
#include <llvm/Transforms/Utils/Cloning.h>

#include <deque>
#include <iostream>
#include <string>

using namespace llvm;

#define DEBUG_TYPE "tas-batch-maker"

namespace tas {

/// Old Function Prototype:
///    Ret Fn(Type1 A, Type2 B BATCH_ARG, Type3 C BATCH_ARG);
/// BatchForm Fn Prototype:
///    void Fn_batch(Type1 A, Type2 * B, Type3 * C, int16_t TAS_BATCHSIZE, Ret * TAS_RETURNS);
void BatchMaker::createBatchedFormFnPrototype() {
  LLVM_DEBUG(errs() << "Function = " << NonBatchFunc->getName() << "\n");

  // Name of the argument to be batched is prefixed with string "batch_arg_i", where
  // i goes from [1, ArgsToBatch.size()]
  std::string prefix { "batch_arg_" }; int i = 0;
  unsigned BatchParamIndex = 0;
  for (auto & Arg : NonBatchFunc->args()) {
    if (ArgsToBatch.find(&Arg) != ArgsToBatch.end()) {
      BatchArgTypes.push_back(PointerType::get(Arg.getType(), 0));
      BatchArgNames.push_back(prefix + std::to_string(i++));
      BatchParamIndices.push_back(BatchParamIndex++);
    } else {
      // Add argument unmodified
      BatchArgTypes.push_back(Arg.getType());
      BatchArgNames.push_back(Arg.getName());
      BatchParamIndex++;
    }
  }

  // Adding Parameter representing actual batch size during run time
  BatchArgTypes.push_back(Type::getInt32Ty(NonBatchFunc->getContext()));
  BatchArgNames.push_back(std::string("TAS_BATCHSIZE"));

  auto RetType = NonBatchFunc->getReturnType();
  // Add pointer as output parameter, where return value is stored.
  BatchArgTypes.push_back(PointerType::get(RetType, 0));
  BatchArgNames.push_back(std::string("TAS_RETURNS"));

  // Create Function prototype
  FunctionType *BatchFuncType = FunctionType::get(RetType, BatchArgTypes, false);
  BatchFunc = Function::Create(BatchFuncType, GlobalValue::ExternalLinkage,
                                        NonBatchFunc->getName() + "_batch", NonBatchFunc->getParent());
}

void BatchMaker::setArgumentNamesInBatchFunc() {
  // Set argument names.
  auto NewArgIt = BatchFunc->arg_begin();
  for (int i = 0; i < BatchFunc->arg_size() - 1; ++i) {
    NewArgIt->setName(BatchArgNames[i]);
    if (i == BatchParamIndices.front()) {
      BatchArgs.push_back(&*NewArgIt);
      BatchParamIndices.pop_front();
    }
    ++NewArgIt;
  }

  NewArgIt->setName(BatchArgNames[BatchFunc->arg_size() - 1]);
  RetArg = &*NewArgIt;
}

void BatchMaker::fillBasicBlocksInBatchFunc() {
  // Store the mapping from old Value to new Value.
  ValueToValueMapTy VMap;
  auto NewArg = BatchFunc->arg_begin();
  for (const Argument & OldArg : NonBatchFunc->args()) {
    VMap[&OldArg] = &*NewArg++;
  }

  SmallVector<ReturnInst*, 8> Returns;  // Ignore returns cloned
  CloneFunctionInto(BatchFunc, NonBatchFunc, VMap, NonBatchFunc->getSubprogram() != nullptr, Returns);
}

void BatchMaker::replaceArgUsesWithBatchArgVal() {
  IRBuilder<> Builder(EntryBB);
  Builder.SetInsertPoint(EntryBB, EntryBB->begin());
  // For each argument, replace all uses.
  for (auto & BatchArg : BatchArgs) {
    auto BatchArgAlloca = Builder.CreateAlloca(BatchArg->getType());
    BatchAllocas.insert(BatchArgAlloca);
    auto StoreI = findFirstUseInStoreInst(BatchArg);

    // Store argument in alloca variable.
    AllocaInst * OldAlloca = nullptr;
    if (StoreI) {
      if ((OldAlloca = dyn_cast<AllocaInst>(StoreI->getPointerOperand())))
        StoreI->setOperand(1, BatchArgAlloca);
    } else {
      continue;
    }

    // Replace old alloca variable uses with new alloca variable.
    // Old variable contained a single pointer, hence access would be single load op.
    // New alloca variable contains double pointer.
    // Hence dereference would be 3 op : load -> getelementptr -> load 
    auto NumUses = OldAlloca->getNumUses();
    while (NumUses > 0) {
      User * U = OldAlloca->user_back();
      Builder.SetInsertPoint(cast<Instruction>(U));
      auto IndexVal = Builder.CreateLoad(IndexVarPtr);
      auto DerefAPtr = Builder.CreateLoad(BatchArgAlloca);
      auto ElemPtr = Builder.CreateGEP(DerefAPtr, IndexVal);
      BatchGEPs.push_back(ElemPtr);
      U->replaceUsesOfWith(OldAlloca, ElemPtr);
      NumUses--;
    }
  }
}

void BatchMaker::updateBasicBlocksInBatchFunc(){
  EntryBB = &BatchFunc->getEntryBlock();
  IRBuilder<> Builder(EntryBB);
  Builder.SetInsertPoint(EntryBB, EntryBB->begin());

  // Batch index variable:
  // int i = 0;
  IndexVarPtr = Builder.CreateAlloca(Builder.getInt32Ty());
  Builder.CreateStore(Builder.getInt32(0), IndexVarPtr);
  // Store Ret parameter in alloca.
  Builder.SetInsertPoint(EntryBB, EntryBB->begin());
  auto RetAlloca = Builder.CreateAlloca(RetArg->getType());
  Builder.CreateStore(RetArg, RetAlloca);

  replaceArgUsesWithBatchArgVal();
}

bool BatchMaker::run() {
  detectBatchingParameters(NonBatchFunc, ArgsToBatch);
  createBatchedFormFnPrototype();
  setArgumentNamesInBatchFunc();
  fillBasicBlocksInBatchFunc();
  updateBasicBlocksInBatchFunc();
  
  return true;
}

} // tas namespace
