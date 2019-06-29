#include "BatchMaker.h"
#include "ForLoop.h"
#include "Util.h"

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/Statistic.h>
#include <llvm/IR/Argument.h>
#include <llvm/IR/CFG.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Transforms/Utils/Cloning.h>

#include <deque>
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
  SmallPtrSet<Value *, 4> ArgsToBatch;
  detectBatchingParameters(OldFunc, ArgsToBatch);

  // Create batch parameters
  SmallVector<Type *, 4> NewParams;
  SmallVector<std::string, 4> ArgNames;
  std::string prefix { "batch_arg_" }; int i = 1;
  unsigned BatchIndex = 0;
  std::deque<unsigned> BatchParamIndices;
  for (auto & Arg : OldFunc->args()) {
    if (ArgsToBatch.find(&Arg) != ArgsToBatch.end()) {
      NewParams.push_back(PointerType::get(Arg.getType(), 0));
      ArgNames.push_back(prefix + std::to_string(i++));
      BatchParamIndices.push_back(BatchIndex++);
    } else {
      NewParams.push_back(Arg.getType());
      ArgNames.push_back(Arg.getName());
      BatchIndex++;
    }
  }

  // Adding Parameter representing actual batch size during run time
  NewParams.push_back(Type::getInt32Ty(OldFunc->getContext()));
  ArgNames.push_back(std::string("TAS_BATCHSIZE"));

  // Create Function prototype
  auto RetType = OldFunc->getReturnType();
  FunctionType *BatchFuncType = FunctionType::get(RetType, NewParams, false);
  NewFunc = Function::Create(BatchFuncType, GlobalValue::ExternalLinkage,
                                        "batch_fn", OldFunc->getParent());

  // Set argument names.
  SmallVector<Value *, 4> BatchedArgs;
  auto NewArgIt = NewFunc->arg_begin();
  for (int i = 0; i < NewFunc->arg_size(); ++i) {
    NewArgIt->setName(ArgNames[i]);
    if (i == BatchParamIndices.front()) {
      BatchedArgs.push_back(&*NewArgIt);
      BatchParamIndices.pop_front();
    }
    ++NewArgIt;
  }

  ValueToValueMapTy VMap;
  auto NewA = NewFunc->arg_begin();
  for (const Argument & A : OldFunc->args()) {
    VMap[&A] = &*NewA++;
  }

  SmallVector<ReturnInst*, 8> Returns;  // Ignore returns cloned
  CloneFunctionInto(NewFunc, OldFunc, VMap, OldFunc->getSubprogram() != nullptr, Returns);

  auto EntryBB = &NewFunc->getEntryBlock();
  IRBuilder<> Builder(EntryBB);
  Builder.SetInsertPoint(EntryBB, EntryBB->begin());

  // For each argument, replace all uses.
  for (auto & A : BatchedArgs) {
    auto APtr = Builder.CreateAlloca(A->getType());
    Builder.CreateStore(A, APtr);
    StoreInst * StoreI = nullptr;
    for (auto * U : A->users()) {
      if (auto * I = dyn_cast<StoreInst>(U)) {
        StoreI = I;
      }
    }

    // Store argument in alloca variable.
    AllocaInst * OldAlloca = nullptr;
    if (StoreI) {
      if ((OldAlloca = dyn_cast<AllocaInst>(StoreI->getPointerOperand())))
        StoreI->setOperand(1, APtr);
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
      errs() << "User " << *U << "\n";
      Builder.SetInsertPoint(cast<Instruction>(U));
      auto DerefAPtr = Builder.CreateLoad(APtr);
      auto ElemPtr = Builder.CreateGEP(DerefAPtr, Builder.getInt64(0) /*FIXME add index var*/);
      U->replaceUsesOfWith(OldAlloca, ElemPtr);
      NumUses--;
    }
  }

  /*
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
  */

  /*
  SmallVector<BasicBlock *, 4> ClonedBlocks (OldFunc->getBasicBlockList().size());
  for (Function::const_iterator BI = OldFunc->begin(), BE = OldFunc->end();
       BI != BE; ++BI) {
    const BasicBlock &BB = *BI;

    // Create a new basic block and copy instructions into it!
    BasicBlock *CBB = CloneBasicBlock(&BB, VMap, "_batch", NewFunc);
    ClonedBlocks.push_back(CBB);

    // Add basic block mapping.
    VMap[&BB] = CBB;

    if (BB.hasAddressTaken()) {
      Constant *OldBBAddr = BlockAddress::get(const_cast<Function*>(OldFunc),
                                              const_cast<BasicBlock*>(&BB));
      VMap[OldBBAddr] = BlockAddress::get(NewFunc, CBB);
    }
  }

  remapInstructionsInBlocks(ClonedBlocks, VMap);
  */


  /*
  for (auto & A : OldFunc->args()) {
    auto & NewValue = *NewArgIt++;
    errs() << "Old Argument = " << A << "New argument = " << NewValue << "\n";
    replaceUsesWithinBB(&A, &NewValue, EntryBB);
  }
  */

  //auto TL0 = TASForLoop(NewFunc->getContext(), , ReturnBlock, "tas.loop." + std::to_string(i), F);
}

} // tas namespace
