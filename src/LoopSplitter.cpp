#include "LoopSplitter.h"
#include "ForLoop.h"
#include "ForLoopV2.h"
#include "Util.h"

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/CFG.h>
#include <llvm/IR/Dominators.h>
#include <llvm/IR/IRBuilder.h>

#include <iostream>
#include <vector>
#include <string>

using namespace std;
using namespace llvm;

#define DEBUG_TYPE "tas-batch-process"

namespace tas {

void LoopSplitter::addBatchArrayForIntermediateVars(Loop * L0) {
  auto Idx = getLoopIndexVar(L0);
  IRBuilder<> Builder (F->getContext());
  for (auto & AV : AnnotatedVars) {
    auto arrayPtr = createArray(F, cast<AllocaInst>(AV)->getAllocatedType(), 32);
    auto NumUses = AV->getNumUses();
    while (NumUses > 0) {
      User * U = AV->user_back();
      if (cast<Instruction>(U)->getParent() == &F->getEntryBlock()) {
        NumUses--;
        continue;
      }

      Builder.SetInsertPoint(cast<Instruction>(U));
      auto ptr = Builder.CreateGEP(arrayPtr, {Builder.getInt64(0), Builder.CreateLoad(Idx)});
      U->replaceUsesOfWith(AV, ptr);
      NumUses--;
    }
  }
}

bool LoopSplitter::prepareForLoopSplit(Function *F, Stats & stat) {
  AnnotatedVars = detectExpPtrVars(F);
  VarUsePoints = detectExpPtrUses(AnnotatedVars);

  for (int i = 0; i < VarUsePoints.size(); ++i) {
    auto & VarUse = VarUsePoints[i];
    //  VarUse is not first instruction
    auto FirstI = VarUse->getParent()->begin();
    assert (&*FirstI != VarUse && "VarUse is not first instruction in basic block");
    auto PrevI = VarUse->getPrevNode();
    Instruction * SplitPoint = nullptr;
    if (isa<GetElementPtrInst>(PrevI)) {
      SplitPoint = PrevI;
    }
    auto Prefetch = insertLLVMPrefetchIntrinsic(F, VarUse, SplitPoint);
    EndBlocks.push_back(Prefetch->getParent());
    Prefetch->getParent()->splitBasicBlock(Prefetch->getNextNode());
    SplitPoints.push_back(SplitPoint ? SplitPoint : VarUse);
  }

  stat.AnnotatedVarsSize = AnnotatedVars.size();
  stat.VarUsePointsSize = VarUsePoints.size();
  return VarUsePoints.size() != 0;
}

bool LoopSplitter::run() {
  bool changed = prepareForLoopSplit(F, stat);
  if (!changed) return false;

  DominatorTree DT(*F);
  LoopInfo LI(DT);

  // If no loops, we are done.
  if (LI.begin() == LI.end()) return false;

  // XXX Assume only one loop for now.
  auto L0 = *LI.begin();

  addBatchArrayForIntermediateVars(L0);

  auto BodyBegin = L0->getHeader()->getTerminator()->getSuccessor(0);
  BodyEnd = L0->getLoopLatch()->getSinglePredecessor();
  EndBlocks.push_back(BodyEnd);

  auto EntryBlock = getPreLoopBlock(L0);
  ExitBlock = L0->getExitBlock();
  auto TripCount = getLoopTripCount(L0);
  assert (ExitBlock && "Loop must have a single exit block!");

  auto ParentLoop = IRLoop(F->getContext());
  ParentLoop.extractLoopSkeleton(L0);
  auto Index = getLoopIndexVar(L0);
  errs() << *Index;

  std::vector<IRLoop> Loops;
  for (int i = 0; i < AnnotatedVars.size(); ++i) {
    Loops.push_back(IRLoop(F->getContext()));
    Loops.back().constructEmptyLoop(TripCount, Index, F);
  }
  Loops.push_back(ParentLoop);

  // Stitch all loop skeletons.
  setSuccessor(&F->getEntryBlock(), Loops.front().getPreHeader());
  for (int i = 0; i < Loops.size()-1; ++i) {
    Loops[i].setExitBlock(Loops[i+1].getPreHeader());
  }
  Loops.back().setExitBlock(ExitBlock);

  std::vector<BasicBlock *> Blocks;
  traverseLoopBody(Blocks, BodyBegin);

  // FIXME One extra set in the end
  LoopBlocks.pop_back();

  /*
  for (auto & Blocks : LoopBlocks) {
    for (auto & BB : Blocks) {
      BB->printAsOperand(errs());
      errs() << "  ";
    }
    errs() << "\n";
  }
  */

  for (int i = 0; i < Loops.size(); ++i) {
    Loops[i].setLoopBlocks(LoopBlocks[i]);
  }

  return true;
}

void LoopSplitter::traverseLoopBody(std::vector<BasicBlock *> & Blocks,
                                    BasicBlock * Start) {
  Blocks.push_back(Start);
  auto Ending = std::find(EndBlocks.begin(), EndBlocks.end(), Start) != EndBlocks.end();
  if (Ending) { 
    LoopBlocks.push_back(Blocks);
    Blocks.clear();
    if (Start == BodyEnd) return;
  }
  for (auto * BB : successors(Start)) {
    traverseLoopBody(Blocks, BB);
  }
}

}
