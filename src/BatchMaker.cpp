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
  auto & Ctx = NonBatchFunc->getContext();

  int i = 0;
  for (auto & Arg : NonBatchFunc->args()) {
    bool IsBatch = ArgsToBatch.find(&Arg) != ArgsToBatch.end();
    BatchFuncArgList.emplace_back(TASArgAttr { IsBatch, i++, Arg.getType(), &Arg,
                                               Arg.getName() }); 
  }

  // Batch size parameter
  BatchFuncArgList.emplace_back(
      TASArgAttr { false, i++, Type::getInt32Ty(Ctx), nullptr, BatchSizeVarName});

  // Return value pointer
  BatchFuncArgList.emplace_back(
      TASArgAttr { false, i++, PointerType::get(NonBatchFunc->getReturnType(), 0),
                   nullptr, ReturnVarName });


  llvm::SmallVector<llvm::Type *, 4> BatchArgTypes;
  for_each(BatchFuncArgList.begin(), BatchFuncArgList.end(),
            [&] (TASArgAttr & Attr) { BatchArgTypes.push_back(Attr.Ty); });

  // Create Function prototype
  std::string Suffix { "_batch" };
  FunctionType *BatchFuncType = FunctionType::get(Type::getVoidTy(Ctx), BatchArgTypes, false);
  BatchFunc = Function::Create(BatchFuncType, GlobalValue::ExternalLinkage,
                               NonBatchFunc->getName() + Suffix, NonBatchFunc->getParent());

  auto AI = BatchFunc->arg_begin();
  for (auto i = 0; i != BatchFunc->arg_size(); ++i) {
    // Setting name helps in debugging the code, it is not mandatory.
    AI->setName(BatchFuncArgList[i].Name);
    // Set Value object for newly created types, overwrite for old types.
    BatchFuncArgList[i].Val = &*AI;
    AI++;
  }
}

void BatchMaker::replaceOldArgUsesWithBatchArgs() {
  SmallVector<Value *, 4> BatchArgs;
  for_each(BatchFuncArgList.begin(), BatchFuncArgList.end(),
            [&] (TASArgAttr & Attr) {
            if (Attr.IsBatch) BatchArgs.push_back(Attr.Val); });

  for (auto & BatchArg : BatchArgs) {
    auto BatchArgAlloca = Builder.CreateAlloca(BatchArg->getType());
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
      auto ElemPtr = Builder.CreateGEP(Builder.CreateLoad(BatchArgAlloca),
                                       Builder.CreateLoad(IdxPtr));
      U->replaceUsesOfWith(OldAlloca, ElemPtr);
      NumUses--;
    }
  }
}

void BatchMaker::storeRetValInPtrArg() {
  // Store Ret parameter in alloca.
  auto RetArg = BatchFuncArgList.back().Val;
  RetAlloca = Builder.CreateAlloca(RetArg->getType());
  Builder.CreateStore(RetArg, RetAlloca);

  // Replace return instruction with storing in return alloca instruction.
  SmallVector<ReturnInst *, 4> Returns;
  getReturnInstList(BatchFunc, Returns);
  for (auto & RI : Returns) {
    Builder.SetInsertPoint(RI);
    auto BatchPtr = Builder.CreateGEP(Builder.CreateLoad(RetAlloca),
                                      Builder.CreateLoad(IdxPtr)); 
    Builder.CreateStore(RI->getReturnValue(), BatchPtr);
    Return = Builder.CreateRetVoid();
    RI->eraseFromParent();
  }
}

void BatchMaker::doBatchTransform(){
  createBatchedFormFnPrototype();

  // First copy over basic blocks without modifying.
  cloneBasicBlocksInto(NonBatchFunc, BatchFunc);

  auto EntryBB = &BatchFunc->front();
  Builder.SetInsertPoint(&EntryBB->front());

  // Create batch index variable, set to 0.
  IdxPtr = Builder.CreateAlloca(Builder.getInt32Ty());
  Builder.CreateStore(Builder.getInt32(0), IdxPtr);

  replaceOldArgUsesWithBatchArgs();

  storeRetValInPtrArg();

  addBatchLoop();
}

void BatchMaker::addBatchLoop() {
  auto * BBM = findBatchBeginMarkerInstruction(BatchFunc);
  if (BBM == nullptr) return; // Do nothing for now. XXX Try auto detection
  auto * SI = BBM->getNextNode(); // This instruction belongs to new block
  auto ParentBB = SI->getParent();
  auto BatchBB = ParentBB->splitBasicBlock(BasicBlock::iterator(*SI), "tas_block");

  // Split at return instruction
  auto ReturnBB = Return->getParent()->splitBasicBlock(BasicBlock::iterator(*Return),
                                                       "exit_block");

  auto BatchSizeVal = BatchFunc->getValueSymbolTable()->lookup(BatchSizeVarName);
  auto TL0 = TASForLoop(BatchFunc->getContext(), ParentBB, ReturnBB,
                        std::string("loop0"), BatchFunc, BatchSizeVal);
  TL0.setLoopBody(BatchBB);

  // Workaround for existence of two index variable.
  // XXX We have two index variable. TASForLoop uses its own variable which is
  // SSA value. Other one is alloca variable. Combine them to have only one.
  // Using workaround for now. In latch block we increment the alloca variable,
  // so that instructions which use alloca variable need not be updated.
  auto Latch = TL0.getLatch();
  Instruction * NonPhiI = nullptr;
  for (auto & I : *Latch) {
    if (!isa<PHINode>(I)) {
      NonPhiI = &I;
      break;
    }
  }
  assert (NonPhiI != nullptr && "Latch block is invalid!");
  Builder.SetInsertPoint(NonPhiI);
  auto IdxVal = Builder.CreateLoad(IdxPtr);
  auto NewIdxVal = Builder.CreateAdd(IdxVal, Builder.getInt32(1));
  Builder.CreateStore(NewIdxVal, IdxPtr);
}

bool BatchMaker::run() {
  detectBatchingParameters(NonBatchFunc, ArgsToBatch);
  doBatchTransform();
  return true;
}

} // tas namespace
