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
#include <string>

using namespace llvm;

#define DEBUG_TYPE "tas-batch-process"

namespace tas {

void LoopSplitter::addAdapterBasicBlocks(Loop * L, Instruction * SP, Value * Idx) {
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

  // Handle diverge blocks
  auto OldHeader = L->getHeader();
  auto OldEntry = cast<BranchInst>(OldHeader->getTerminator())->getSuccessor(0);

  SmallVector<BasicBlock *, 4> TopBlocks;
  SmallVector<BasicBlock *, 4> BottomBlocks;
  LoopBodyTraverser LBT (L);
  LBT.traverse(TopBlocks, OldEntry, DistBB);
  LBT.traverse(BottomBlocks, DistBB, L->getLoopLatch());

  SmallVector<BasicBlock *, 4> DivergeBlocks;
  for (auto & BB : TopBlocks) {
    for (auto * Succ : successors(BB)) {
      Succ->printAsOperand(errs());
      if (std::find(BottomBlocks.begin(), BottomBlocks.end(), Succ) != BottomBlocks.end())
        DivergeBlocks.push_back(BB);
    }
  }

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
  auto VarUsePoints = detectExpPtrUses(AnnotatedVars);

  // Add unique id to each basic block
  unsigned i = 0;
  IRBuilder<> Builder(F->getContext());
  auto SetIntValForBB = [&] (const auto & BB) {
                        BBToId.insert(std::make_pair(&BB, Builder.getInt32(++i)));
  };
  for_each(*F, SetIntValForBB);

  for_each(VarUsePoints,
      [&] (auto & VarUse) { insertLLVMPrefetchIntrinsic(F, VarUse); });

  for_each(VarUsePoints,
      [&] (auto & VarUse) { addAdapterBasicBlocks(L0, VarUse, Idx); });
  
  stat.AnnotatedVarsSize = AnnotatedVars.size();
  stat.VarUsePointsSize = VarUsePoints.size();
  return VarUsePoints.size() != 0;
}

void LoopSplitter::doLoopSplit(Function * F, Loop * L0, BasicBlock * SplitBlock) {
  auto OldHeader = L0->getHeader();
  auto OldEntry = cast<BranchInst>(OldHeader->getTerminator())->getSuccessor(0);
  auto PreLoopBB = getPreLoopBlock(L0);
  auto PostLoopBB = cast<BranchInst>(OldHeader->getTerminator())->getSuccessor(1);
  auto MidBlock = SplitBlock->getUniqueSuccessor();

  auto LBT = LoopBodyTraverser(L0);

  // Collect Blocks in range [OldEntry, MidBlock)
  SmallVector<BasicBlock *, 4> TopLoopBlocks;
  LBT.traverse(TopLoopBlocks, OldEntry, MidBlock);

  // Collect Blocks in range [MidBlock, Latch)
  SmallVector<BasicBlock *, 4> BottomLoopBlocks;
  LBT.traverse(BottomLoopBlocks, MidBlock, L0->getLoopLatch());

  auto BottomLoop = IRLoop();
  BottomLoop.extractLoopSkeleton(L0);

  auto TopLoop = IRLoop();
  TopLoop.constructEmptyLoop(getLoopTripCount(L0), BottomLoop.getHeader());

  TopLoop.setLoopBlocks(TopLoopBlocks);
  BottomLoop.setLoopBlocks(BottomLoopBlocks);

  setSuccessor(PreLoopBB, TopLoop.getPreHeader());
  setSuccessor(TopLoop.getHeader(), BottomLoop.getHeader(), 1); 
  setSuccessor(BottomLoop.getHeader(), PostLoopBB, 1);

  /*
  // If FalseBB is terminating instruction, use latch block as target instead.
  SmallVector<BasicBlock *, 4> Returns;
  getReturnBlocks(F, Returns);
  for (auto & Case : SwitchI->cases()) {
    if (find(Returns, Case.getCaseSuccessor()) != Returns.end()) {
      Case.setSuccessor(L0->getLoopLatch());
    }
  }
  */
}

/*
  DenseSet<BasicBlock *> Blocks;
  auto TruePathBB = NewHeader->getTerminator()->getSuccessor(0);
  visitSuccessor(Blocks, TruePathBB, NewLatch);

  auto BB = Blocks.begin();
  auto BE = Blocks.end();
  auto IB = (*BB)->begin();
  auto IE = (*BB)->end();
  while (BB != BE) {
    while (IB != IE) {
      auto UB = (*IB).user_begin();
      auto UE = (*IB).user_end();
      while (UB != UE) {
        auto * U = *UB++;
        if (auto * Inst = dyn_cast<Instruction>(U)) {
          if (Blocks.find(Inst->getParent()) == Blocks.end()) {
            auto arrayPtr = createArray(F, (*IB).getType(), 32);

            Builder.SetInsertPoint((*IB).getNextNode());
            IndexVarVal = Builder.CreateLoad(IndexVar);
            auto ptr = Builder.CreateGEP(arrayPtr, {Builder.getInt64(0), IndexVarVal});
            auto str = Builder.CreateStore(&*IB, ptr);
            errs() << *ptr << "\n" << *str << "\n"; 
          }
        }
      }
      ++IB;
    }
    ++BB;
  }
}
*/

bool LoopSplitter::run() {
  // If no loops, we are done.
  if (LI->begin() == LI->end()) return false;

  // XXX Assume only one loop for now.
  auto L0 = *LI->begin();

  ExitBlock = L0->getExitBlock();
  assert (ExitBlock && "Loop must have a single exit block!");

  auto ParentLoop = IRLoop();
  ParentLoop.analyze(L0);

  bool changed = prepareForLoopSplit(F, L0, stat);
  if (!changed) return false;

//  F->print(errs());

  DominatorTree DT (*F);
  LoopInfo NewLI (DT);
  L0 = *NewLI.begin();
  if (NewLI.begin() == NewLI.end()) {
    errs() << "Loop info lost, something wrong with preparation\n";
    return false;
  }
  auto & SplitBB = LoopSplitEdgeBlocks.front();
//  doLoopSplit(F, L0, SplitBB);

  return true;
}

}
