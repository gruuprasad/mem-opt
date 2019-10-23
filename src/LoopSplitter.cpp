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

void LoopSplitter::addAdapterBasicBlocks(Instruction * SP) {
  auto TopHalf = SP->getParent();
  auto BottomHalf = TopHalf->splitBasicBlock(SP->getNextNode());

  auto CollectBB = BasicBlock::Create(F->getContext(), "collector", F, BottomHalf);
  auto DistBB = BasicBlock::Create(F->getContext(), "distributor", F, BottomHalf);

  BranchInst::Create(DistBB, CollectBB);
  setSuccessor(TopHalf, CollectBB);
  LoopSplitEdgeBlocks.push_back(CollectBB);

  // General case: Find all edges from basic blocks above tophalf and connect it to
  // CollectBB, use switch Inst
  IRBuilder<> Builder(&F->getEntryBlock().front());
  auto BrTargetAlloca = Builder.CreateAlloca(Builder.getInt32Ty());

  Builder.SetInsertPoint(DistBB);
  auto BrVal = Builder.CreateLoad(BrTargetAlloca);
  auto SwitchI = Builder.CreateSwitch(BrVal, BottomHalf);

  // XXX We assume now CFG we have is the one after block predication transformation.
  SmallVector<BasicBlock *, 4> DivergeBlocks;
  SmallVector<std::pair<Value *, BasicBlock *>, 4> TargetBlocks;
  DivergeBlocks.push_back(TopHalf->getUniquePredecessor());

  for (auto & DivergeBB : DivergeBlocks) {
    auto TermI = cast<BranchInst>(DivergeBB->getTerminator());
    auto Cond = TermI->getCondition();

    Builder.SetInsertPoint(TermI);
    auto TgtBB = TermI->getSuccessor(1);
    auto TgtBBVal = BBToId[TgtBB];

    Builder.CreateStore(TgtBBVal, BrTargetAlloca);
    TermI->setSuccessor(1, CollectBB);
    SwitchI->addCase(TgtBBVal, TgtBB);
  }
}

bool LoopSplitter::prepareForLoopSplit(Function *F, Stats & stat) {
  auto AnnotatedVars = detectExpPtrVars(F);
  auto VarUsePoints = detectExpPtrUses(AnnotatedVars);

  // Add unique id to each basic block
  unsigned i = 0;
  IRBuilder<> Builder(F->getContext());
  for_each(*F,
      [&] (const auto & BB) { BBToId.insert(std::make_pair(&BB, Builder.getInt32(++i))); });

  for_each(VarUsePoints,
      [&] (auto & VarUse) { addAdapterBasicBlocks(VarUse); });

  for_each(VarUsePoints,
      [&] (auto & VarUse) { insertLLVMPrefetchIntrinsic(F, VarUse); });

  stat.AnnotatedVarsSize = AnnotatedVars.size();
  stat.VarUsePointsSize = VarUsePoints.size();
  return VarUsePoints.size() != 0;
}

bool LoopSplitter::run() {
  bool changed = prepareForLoopSplit(F, stat);
  if (!changed) return false;

  // If no loops, we are done.
  if (LI->begin() == LI->end()) return false;

  // XXX Assume only one loop for now.
  auto L0 = *LI->begin();

  //assert (L0->getLoopPreheader() && "Loop doesn't have preheader!");

  return true;
}

}
