#include "LoopSplitter.h"
#include "ForLoop.h"
#include "Util.h"

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/CFG.h>
#include <llvm/IR/Dominators.h>
#include <llvm/IR/IRBuilder.h>

#include <iostream>
#include <string>

using namespace llvm;

#define DEBUG_TYPE "tas-batch-process"

namespace tas {

void LoopSplitter::addAdapterBasicBlocks(Instruction * SP, Value * Idx) {
  auto TopHalf = SP->getParent();
  auto BottomHalf = TopHalf->splitBasicBlock(SP->getNextNode());

  auto CollectBB = BasicBlock::Create(F->getContext(), "collector", F, BottomHalf);
  auto DistBB = BasicBlock::Create(F->getContext(), "distributor", F, BottomHalf);

  BranchInst::Create(DistBB, CollectBB);
  setSuccessor(TopHalf, CollectBB);
  setSuccessor(CollectBB, DistBB);
  LoopSplitEdgeBlocks.push_back(CollectBB);

  // General case: Find all edges from basic blocks above tophalf
  // and connect it to CollectBB, use switch Inst
  IRBuilder<> Builder(&F->getEntryBlock().front());
  auto BrTgtArray = createArray(F, Builder.getInt32Ty(), 32 /*XXX Max Batch size*/);
  //auto BrTargetAlloca = Builder.CreateAlloca(Builder.getInt32Ty());

  Builder.SetInsertPoint(DistBB);
  auto BrValPtr = Builder.CreateGEP(BrTgtArray, {Builder.getInt64(0), Idx});
  auto BrVal = Builder.CreateLoad(BrValPtr);
  auto SwitchI = Builder.CreateSwitch(BrVal, BottomHalf);

  // XXX We assume now CFG we have is the one after block
  // predication transformation.
  SmallVector<BasicBlock *, 4> DivergeBlocks;
  SmallVector<std::pair<Value *, BasicBlock *>, 4> TargetBlocks;
  DivergeBlocks.push_back(TopHalf->getUniquePredecessor());

  for (auto & DivergeBB : DivergeBlocks) {
    auto TermI = cast<BranchInst>(DivergeBB->getTerminator());
    auto Cond = TermI->getCondition();

    Builder.SetInsertPoint(TermI);
    auto TgtBB = TermI->getSuccessor(1);
    auto TgtBBVal = BBToId[TgtBB];

    auto BrValPtr = Builder.CreateGEP(BrTgtArray, {Builder.getInt64(0), Idx});
    Builder.CreateStore(TgtBBVal, BrValPtr);
    TermI->setSuccessor(1, CollectBB);
    SwitchI->addCase(TgtBBVal, TgtBB);
  }
}

bool LoopSplitter::prepareForLoopSplit(Function *F, Stats & stat, Value * Idx) {
  auto AnnotatedVars = detectExpPtrVars(F);
  auto VarUsePoints = detectExpPtrUses(AnnotatedVars);

  // Add unique id to each basic block
  unsigned i = 0;
  IRBuilder<> Builder(F->getContext());
  for_each(*F,
      [&] (const auto & BB) {
      BBToId.insert(std::make_pair(&BB, Builder.getInt32(++i)));
    });

  for_each(VarUsePoints,
      [&] (auto & VarUse) { addAdapterBasicBlocks(VarUse, Idx); });

  for_each(VarUsePoints,
      [&] (auto & VarUse) { insertLLVMPrefetchIntrinsic(F, VarUse); });

  stat.AnnotatedVarsSize = AnnotatedVars.size();
  stat.VarUsePointsSize = VarUsePoints.size();
  return VarUsePoints.size() != 0;
}

Value * getLoopTripCount(Loop * L0) {
  auto Header = L0->getHeader();
  auto Cond = cast<BranchInst>(Header->getTerminator())->getCondition();
  return cast<LoadInst>(cast<ICmpInst>(Cond)->getOperand(1))->getOperand(0);
}

void doLoopSplit(Function * F, Loop * L0, BasicBlock * SplitBlock) {
  auto OldHeader = L0->getHeader();
  auto ExitBlock = OldHeader->getTerminator()->getSuccessor(1);
  auto DistBB = SplitBlock->getUniqueSuccessor();

  BasicBlock * EntryBlock = nullptr;
  for (auto BB : predecessors(OldHeader)) {
    if (BB != L0->getLoopLatch())
      EntryBlock = BB;
  }

  auto NewPreHeader = BasicBlock::Create(F->getContext(), "preheader", F);
  auto NewHeader = BasicBlock::Create(F->getContext(), "header", F);
  auto NewLatch = BasicBlock::Create(F->getContext(), "latch", F);

  auto IndexVar = getLoopIndexVar(L0);
  auto TripCount = cast<AllocaInst>(getLoopTripCount(L0));
  errs() << *IndexVar << "  " << *TripCount << "\n";

  IRBuilder<> Builder(NewPreHeader);
  Builder.CreateStore(Builder.getInt32(0), IndexVar);
  Builder.CreateBr(NewHeader);

  Builder.SetInsertPoint(NewLatch);
  auto BI = Builder.CreateBr(NewHeader);
  addIncrementIndexOp(IndexVar, BI);

  Builder.SetInsertPoint(NewHeader);
  auto IndexVarVal = Builder.CreateLoad(IndexVar);
  auto TripCountVal = Builder.CreateLoad(TripCount);
  auto * icmp = Builder.CreateICmpSLT(IndexVarVal, TripCountVal,
                                      "loop-predicate");
  auto NewEntryBB = cast<BranchInst>(OldHeader->getTerminator())->getSuccessor(0);
  Builder.CreateCondBr(icmp, NewEntryBB, L0->getHeader());

  SplitBlock->getTerminator()->setSuccessor(0, NewLatch);
  OldHeader->getTerminator()->setSuccessor(0, DistBB);
  OldHeader->getTerminator()->setSuccessor(1, ExitBlock);
  setSuccessor(EntryBlock, NewPreHeader);
}

bool LoopSplitter::run() {

  // If no loops, we are done.
  if (LI->begin() == LI->end()) return false;


  // XXX Assume only one loop for now.
  auto L0 = *LI->begin();
  auto IndexVar = getLoopIndexVar(L0);

  bool changed = prepareForLoopSplit(F, stat, IndexVar);
  if (!changed) return false;

  auto & SplitBB = LoopSplitEdgeBlocks.front();
  doLoopSplit(F, L0, SplitBB);

  F->print(errs());

  return true;
}

}
