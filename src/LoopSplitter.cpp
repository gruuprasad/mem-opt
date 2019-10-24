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
  auto BottomHalf = TopHalf->splitBasicBlock(SP);

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

  Builder.SetInsertPoint(DistBB);
  auto IdxVal = Builder.CreateLoad(Idx);
  auto IdxVal64 = Builder.CreateSExtOrBitCast(IdxVal, Builder.getInt64Ty());
  auto BrValPtr = Builder.CreateGEP(BrTgtArray, {Builder.getInt64(0), IdxVal64});
  auto BrVal = Builder.CreateLoad(BrValPtr);
  SwitchI = Builder.CreateSwitch(BrVal, BottomHalf);

  // XXX We assume now CFG we have is the one after block
  // predication transformation.
  SmallVector<BasicBlock *, 4> DivergeBlocks;
  DivergeBlocks.push_back(TopHalf->getUniquePredecessor());

  for (auto & DivergeBB : DivergeBlocks) {
    auto TermI = cast<BranchInst>(DivergeBB->getTerminator());
    auto Cond = TermI->getCondition();

    Builder.SetInsertPoint(TermI);

    auto FalseBB = TermI->getSuccessor(1);
    auto TgtBBVal = Builder.CreateSelect(Cond,
                                        BBToId[TermI->getSuccessor(0)],
                                        BBToId[FalseBB]);

    auto IdxVal = Builder.CreateLoad(Idx);
    auto IdxVal64 = Builder.CreateSExtOrBitCast(IdxVal, Builder.getInt64Ty());
    auto BrValPtr = Builder.CreateGEP(BrTgtArray, {Builder.getInt64(0), IdxVal64});
    Builder.CreateStore(TgtBBVal, BrValPtr);
    TermI->setSuccessor(1, CollectBB);
    SwitchI->addCase(BBToId[FalseBB], FalseBB);
  }
}

bool LoopSplitter::prepareForLoopSplit(Function *F, Loop * L0, Stats & stat) {
  auto Idx = getLoopIndexVar(L0);
  auto AnnotatedVars = detectExpPtrVars(F);
  errs() << "AnnotatedVars = " << AnnotatedVars.size() << "  " << *AnnotatedVars.front() << "\n";
  auto VarUsePoints = detectExpPtrUses(AnnotatedVars);

  // Add unique id to each basic block
  unsigned i = 0;
  IRBuilder<> Builder(F->getContext());
  for_each(*F,
      [&] (const auto & BB) {
      BBToId.insert(std::make_pair(&BB, Builder.getInt32(++i)));
    });

  for_each(VarUsePoints,
      [&] (auto & VarUse) { insertLLVMPrefetchIntrinsic(F, VarUse); });

  for_each(VarUsePoints,
      [&] (auto & VarUse) { addAdapterBasicBlocks(VarUse, Idx); });
  
  stat.AnnotatedVarsSize = AnnotatedVars.size();
  stat.VarUsePointsSize = VarUsePoints.size();
  return VarUsePoints.size() != 0;
}

Value * getLoopTripCount(Loop * L0) {
  auto Header = L0->getHeader();
  auto Cond = cast<BranchInst>(Header->getTerminator())->getCondition();
  return cast<LoadInst>(cast<ICmpInst>(Cond)->getOperand(1))->getOperand(0);
}

void LoopSplitter::doLoopSplit(Function * F, Loop * L0, BasicBlock * SplitBlock) {
  auto OldHeader = L0->getHeader();
  auto DistBB = SplitBlock->getUniqueSuccessor();

  BasicBlock * EntryBlock = nullptr;
  for (auto BB : predecessors(OldHeader)) {
    if (BB != L0->getLoopLatch())
      EntryBlock = BB;
  }

  auto OldPreHeader = BasicBlock::Create(F->getContext(), "oldpreheader", F);
  auto NewPreHeader = BasicBlock::Create(F->getContext(), "preheader", F);
  auto NewHeader = BasicBlock::Create(F->getContext(), "header", F);
  auto NewLatch = BasicBlock::Create(F->getContext(), "latch", F);

  auto IndexVar = getLoopIndexVar(L0);
  auto TripCount = cast<AllocaInst>(getLoopTripCount(L0));

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
  Builder.CreateCondBr(icmp, NewEntryBB, OldPreHeader);

  SplitBlock->getTerminator()->setSuccessor(0, NewLatch);
  OldHeader->getTerminator()->setSuccessor(0, DistBB);
  OldHeader->getTerminator()->setSuccessor(1, ExitBlock);
  setSuccessor(EntryBlock, NewPreHeader);

  Builder.SetInsertPoint(OldPreHeader);
  errs() << *IndexVar << "\n";
  Builder.CreateStore(Builder.getInt32(0), IndexVar);
  Builder.CreateBr(OldHeader);

  // If FalseBB is terminating instruction, use latch block as target instead.
  SmallVector<BasicBlock *, 4> Returns;
  getReturnBlocks(F, Returns);
  for (auto & Case : SwitchI->cases()) {
    if (find(Returns, Case.getCaseSuccessor()) != Returns.end()) {
      Case.setSuccessor(L0->getLoopLatch());
    }
  }
}

bool LoopSplitter::run() {
  // If no loops, we are done.
  if (LI->begin() == LI->end()) return false;

  // XXX Assume only one loop for now.
  auto L0 = *LI->begin();

  ExitBlock = L0->getExitBlock();
  assert (ExitBlock && "Loop must have a single exit block!");

  errs() << "Running prepareForLoopSplit\n";
  bool changed = prepareForLoopSplit(F, L0, stat);
  if (!changed) return false;

  auto & SplitBB = LoopSplitEdgeBlocks.front();
  errs() << "running  doLoopSplit\n";
  doLoopSplit(F, L0, SplitBB);

  return true;
}

}
