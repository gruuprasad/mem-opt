#include "BatchMaker.h"
#include "LoopSplitter.h"
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
#include <llvm/IR/Verifier.h>
#include <llvm/Transforms/Utils/BasicBlockUtils.h>
#include <llvm/Transforms/Utils/Cloning.h>

#include <deque>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace llvm;

#define DEBUG_TYPE "tas-batch-maker"

namespace tas {

/// If function parameter annotated as BATCH_ARG and if it is a double pointer,
/// Then additional pointer indirection not added, but arg itself treated as
/// pointer array.
/// Old Function Prototype:
///    Ret Fn(Type1 A, Type2 B BATCH_ARG, Type3 C BATCH_ARG);
/// BatchForm Fn Prototype:
///    void Fn_batch(Type1 A, Type2 * B, Type3 * C, int16_t TAS_BATCHSIZE, Ret * TAS_RETURNS);
void BatchMaker::createBatchedFormFnPrototype(vector<TASArgAttr> & BatchFuncArgList) {
  LLVM_DEBUG(errs() << "Function = " << NonBatchFunc->getName() << "\n");
  auto & Ctx = NonBatchFunc->getContext();

  int i = 0;
  for (auto & Arg : NonBatchFunc->args()) {
    bool IsBatch = ArgsToBatch.find(&Arg) != ArgsToBatch.end();
    BatchFuncArgList.emplace_back(TASArgAttr { IsBatch, i++, Arg.getType(), &Arg,
                                               Arg.getName() }); 
  }

  // Batch size parameter
  BatchSizeArgPos = i;
  BatchFuncArgList.emplace_back(
      TASArgAttr { false, i++, Type::getInt32Ty(Ctx), nullptr, BatchSizeVarName});

  // Return value pointer
  IsRetTyVoid = NonBatchFunc->getReturnType()->isVoidTy();
  if (!IsRetTyVoid)
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

void BatchMaker::replaceOldArgUsesWithBatchArgs(vector<TASArgAttr> & BatchFuncArgList,
                                                AllocaInst * IdxPtr) {
  auto EntryBB = &BatchFunc->front();

  SmallVector<std::pair<Value *, bool>, 4> BatchArgs;
  for_each(BatchFuncArgList.begin(), BatchFuncArgList.end(),
      [&] (TASArgAttr & Attr) {
      if (Attr.IsBatch) BatchArgs.push_back(make_pair(Attr.Val, Attr.IsDoublePtr)); });

  Builder.SetInsertPoint(&EntryBB->front());
  BatchSizeAlloca = Builder.CreateAlloca(BatchFuncArgList[BatchSizeArgPos].Ty);
  Builder.CreateStore(BatchFuncArgList[BatchSizeArgPos].Val, BatchSizeAlloca);

  for (auto & BatchArgPair : BatchArgs) {
    Builder.SetInsertPoint(&EntryBB->front());
    auto & BatchArg = BatchArgPair.first;
    auto IsDoublePtr = BatchArgPair.second;
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
    // Old variable contained a single pointer, hence access would be
    // single load op.
    // New alloca variable contains double pointer.
    // Hence dereference would be 3 op : load -> getelementptr -> load 
    auto NumUses = OldAlloca->getNumUses();
    while (NumUses > 0) {
      User * U = OldAlloca->user_back();
      if (IsDoublePtr) {
        U->replaceUsesOfWith(OldAlloca, BatchArgAlloca);
        auto Next = cast<Instruction>(U)->getNextNode();
        if (isa<GetElementPtrInst>(Next)) {
          Builder.SetInsertPoint(Next);
          auto NewGEP = Builder.CreateGEP(cast<GetElementPtrInst>(Next)->getPointerOperand(),
                                        Builder.CreateLoad(IdxPtr));
          Next->replaceAllUsesWith(NewGEP);
          Next->eraseFromParent();
        }
      } else {
        Builder.SetInsertPoint(cast<Instruction>(U));
        auto ElemPtr = Builder.CreateGEP(Builder.CreateLoad(BatchArgAlloca),
            Builder.CreateLoad(IdxPtr));
        U->replaceUsesOfWith(OldAlloca, ElemPtr);
      }
      NumUses--;
    }
  }
}

BasicBlock * BatchMaker::storeRetValInPtrArg(Argument * RetArg, 
                                            AllocaInst * IdxPtr) {
  auto EntryBB = &BatchFunc->front();
  Builder.SetInsertPoint(&EntryBB->front());

  // Store Ret parameter in alloca.
  auto RetAlloca = Builder.CreateAlloca(RetArg->getType());
  Builder.CreateStore(RetArg, RetAlloca);

  // Store return value in RetAlloca. Now instead return void.
  SmallVector<ReturnInst *, 4> Returns;
  getReturnInstList(BatchFunc, Returns);

  BasicBlock * RetBlock = nullptr;
  if (Returns.size() > 1) {
    RetBlock = BasicBlock::Create(BatchFunc->getContext(), "exit_block",
                                  BatchFunc);
    ReturnInst::Create(BatchFunc->getContext());
  }

  // XXX Rewrite this mess.
  for (auto & RI : Returns) {
    Builder.SetInsertPoint(RI);
    auto BatchPtr = Builder.CreateGEP(Builder.CreateLoad(RetAlloca),
        Builder.CreateLoad(IdxPtr)); 
    Builder.CreateStore(RI->getReturnValue(), BatchPtr);
    if (RetBlock) {
      BranchInst::Create(RetBlock, RI->getParent());
    } else {
      auto RetI = Builder.CreateRetVoid();
      RetBlock = RetI->getParent()->splitBasicBlock(RetI->getIterator(),
                                                    "exit_block");
    }
    RI->eraseFromParent();
  }

  return RetBlock;
}

void BatchMaker::addBatchLoop(BasicBlock * RetBlock, AllocaInst * IdxPtr) {
  auto * BBM = findBatchBeginMarkerInstruction(BatchFunc);
  if (BBM == nullptr) return; // Do nothing for now. XXX Try auto detection

  auto * SI = BBM->getNextNode(); // This instruction belongs to new block
  auto ParentBB = SI->getParent();
  auto BatchBB = ParentBB->splitBasicBlock(BasicBlock::iterator(*SI),
                                           "tas_block");

  vector<BasicBlock *> ExitingBlocks;
  for (auto * BB : predecessors(RetBlock)) {
    ExitingBlocks.push_back(BB);
  }

  assert (ExitingBlocks.size() >= 1 &&
          "Return block must have atleast one predecessor");
  BasicBlock * UniqueExitingBlock = ExitingBlocks[0];
  if (ExitingBlocks.size() > 1) {
    UniqueExitingBlock = BasicBlock::Create(BatchFunc->getContext(), "ExitingBlock",
                                            BatchFunc, RetBlock);
    for_each(ExitingBlocks.begin(), ExitingBlocks.end(),
             [&] (BasicBlock * BB) { setSuccessor(BB, UniqueExitingBlock); });
  }

  auto TL0 = TASForLoop(BatchFunc->getContext(), ParentBB, RetBlock,
                        std::string("loop0"), BatchFunc, BatchSizeAlloca, IdxPtr);

  TL0.setLoopBody(BatchBB, UniqueExitingBlock);
}

BasicBlock * unifyReturnBlock(Function * F) {
  SmallVector<ReturnInst *, 4> Returns;
  getReturnInstList(F, Returns);

  BasicBlock * RetBlock = nullptr;
  if (Returns.size() == 1) {
    RetBlock = Returns[0]->getParent()->splitBasicBlock(Returns[0]->getIterator(),
                                                    "exit_block");
    return RetBlock;
  }

  RetBlock = BasicBlock::Create(F->getContext(), "exit_block", F);
  ReturnInst::Create(F->getContext());

  for (auto & RI : Returns) {
    BranchInst::Create(RetBlock, RI->getParent());
    RI->eraseFromParent();
  }

  return RetBlock;
}

void BatchMaker::doBatchTransform() {
  vector<TASArgAttr> BatchFuncArgList;
  createBatchedFormFnPrototype(BatchFuncArgList);

  // First copy over basic blocks without modifying.
  cloneBasicBlocksInto(NonBatchFunc, BatchFunc);
  
  // Create batch index variable, set to 0.
  auto EntryBB = &BatchFunc->front();
  Builder.SetInsertPoint(&EntryBB->front());
  auto IdxPtr = Builder.CreateAlloca(Builder.getInt32Ty());
  Builder.CreateStore(Builder.getInt32(0), IdxPtr);

  replaceOldArgUsesWithBatchArgs(BatchFuncArgList, IdxPtr);

  BasicBlock * RetBlock = nullptr;
  if (IsRetTyVoid) {
    RetBlock = unifyReturnBlock(BatchFunc);
  } else {
    RetBlock = storeRetValInPtrArg(BatchFuncArgList.back().Val, IdxPtr);
  }
  assert(RetBlock != nullptr);

  addBatchLoop(RetBlock, IdxPtr);
}

Function * BatchMaker::run() {
  detectBatchParameters(NonBatchFunc, ArgsToBatch);
  doBatchTransform();
  return BatchFunc;
}

} // tas namespace
