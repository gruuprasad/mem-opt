#include "LoopSplitter.h"
#include "ForLoop.h"
#include "Util.h"

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/Statistic.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/CFG.h>
#include <llvm/IR/IRBuilder.h>

#include <iostream>
#include <string>

using namespace llvm;

#define DEBUG_TYPE "tas-batch-process"

STATISTIC(NumAnnotatedVariables, "Number of annotated variables detected");
STATISTIC(NumPrefetchInsts, "Number of Prefetch instructions added");

namespace tas {

AllocaInst * getLoopIndexVar(Loop * L) {
  AllocaInst * Index = nullptr;
  auto LatchBB = L->getLoopLatch();
  assert(LatchBB && "Latch block can't be null!");

  // Find index increment instruction.
  // XXX Assume latch block contains only one add instruction, verify it.
  auto It = find_if(*LatchBB,
          [&] (const Instruction & I) {
          return (isa<BinaryOperator>(I) &&
                  I.getOpcode() == Instruction::Add &&
                  cast<ConstantInt>(I.getOperand(1))->equalsInt(1)) ? true : false; });

  Index = cast<AllocaInst>(cast<LoadInst>(It->getOperand(0))->getOperand(0));
  return Index;
}

bool LoopSplitter::run() {
  detectExpPtrVars(F, AnnotatedVariables);
  NumAnnotatedVariables += AnnotatedVariables.size();
  findVariableUsePoints();

  if (AnnotatedVariables.empty())
    return false;

  if (LI->begin() == LI->end()) return false;

  Loop * L0 = *LI->begin(); // XXX Split only first top level loop
  splitLoop(L0);

  return true;
}

void LoopSplitter::splitLoop(Loop * L0) {
  // Original loop is retained, but it's body is split on each iteration.
  // One part becomes part of the new loop and rest remains with the old loop.
  auto * L0_Head = L0->getHeader(); //Doesn't change.
  Value * L0_IndexVar = nullptr;

  //TODO Add check for loop-simplified form.
  // Preheader changes on every new loop insertion
  auto * PreHeader = L0->getLoopPreheader();

  // If there is no phi node, that means loop is not in loop-simplified form.
  // we don't do anything in that case.
  bool canonVar = true;
  if (!isa<PHINode>(L0->getHeader()->begin())) {
    L0_IndexVar = getLoopIndexVar(L0);
    canonVar = false;
  }
  else
    L0_IndexVar = L0->getCanonicalInductionVariable();

  // Convert scalar alloca variable into array form
  auto tripCount = TASForLoop::getLoopTripCount();
  IRBuilder<> Builder(F->getContext());
  for (auto & AI : AnnotatedVariables) {
    // Check the size of the struct type
    auto ElementType = AI->getType();
    while (ElementType->isPointerTy()) {
      ElementType = ElementType->getPointerElementType();
    }

    auto arrayPtr = createArray(F, cast<AllocaInst>(AI)->getAllocatedType(), tripCount);
    auto NumUses = AI->getNumUses();
    while (NumUses > 0) {
      User * U = AI->user_back();
      if (cast<Instruction>(U)->getParent() == &F->getEntryBlock()) {
        NumUses--;
        continue;
      }
      if (isa<StoreInst>(U)) {
        PrefetchAddresses.push_back(cast<StoreInst>(U)->getOperand(0));
      }
      Builder.SetInsertPoint(cast<Instruction>(U));
      Value * IdxVal = L0_IndexVar;
      if (!canonVar)
        IdxVal = Builder.CreateLoad(L0_IndexVar);
      auto ptr = Builder.CreateGEP(arrayPtr, {Builder.getInt64(0), IdxVal});
      U->replaceUsesOfWith(AI, ptr);
      NumUses--;
    }
  }

  // Remove phi node entries if any
  PHINode * PN = nullptr;
  if (canonVar) {
    PN = &*(L0->getHeader()->phis().begin());
    PN->removeIncomingValue(L0->getLoopPreheader());
  }

  // Split basic block at annotated variable def points.
  unsigned int i = 0;
  auto PA_It= PrefetchAddresses.begin();
  for (auto & DP : AnnotatedVariableDefPoints) {
 
    // Insert new loop
    auto TL0 = TASForLoop(F->getContext(), PreHeader, L0_Head, "tas.loop." + std::to_string(i), F,
        ConstantInt::get(Type::getInt32Ty(F->getContext()), TASForLoop::getLoopTripCount()), cast<AllocaInst>(L0_IndexVar));

    // Split old loop body into two parts. Add one part to newly created loop.
    auto * ParentBody = DP->getParent();

    // XXX Revisit this point
    // Currently at split point odd GEP instruction appears. If we split at that point strictly,
    // then it introduces silly value dependence between two loops.
    // GEP in itself not a computation, it must be assoicated with following instruction. hence it will not
    // be considered for split point, rather instruction above GEP instruction is the possible candidate.
    auto * SplitPoint = DP;
    while (isa<GetElementPtrInst>(SplitPoint->getPrevNode())) {
      SplitPoint = SplitPoint->getPrevNode();
    }

    // Insert Prefetch instruction
    assert (PA_It != PrefetchAddresses.end());
    insertLLVMPrefetchIntrinsic(F, SplitPoint, *PA_It);
    ++NumPrefetchInsts;

    auto * NewBody = ParentBody->splitBasicBlock(SplitPoint, "batch_edge_" + std::to_string(i));
    ParentBody->replaceAllUsesWith(NewBody);

    replaceUsesWithinBB(L0_IndexVar, TL0.getIndexVariable(), ParentBody);
    TL0.setLoopBody(ParentBody);

    // XXX Assumption here is that each loop body will be single basic block.
    // Otherwise, further analysis need to be made to set proper index 
    // variable when temporary array access at different use instructions.
    // check if there is a value dependence between two loops.
    fixValueDependenceBetWeenLoops(&TL0, L0_IndexVar);

    // Update old loop preheader block
    PreHeader = TL0.getHeader();
    ++i;
    ++PA_It;
  }

  // Add new phi node edge.
  if (canonVar)
    PN->addIncoming(ConstantInt::get(PN->getType(), 0), PreHeader);
}

void LoopSplitter::fixValueDependenceBetWeenLoops(TASForLoop * NewLoop, Value * OldIndex) {
  BasicBlock * Body = NewLoop->getBody();
  assert (Body != nullptr && "Loop must have a body");
  for (Instruction & I : *Body) {
    for (auto * U : I.users()) {
      if (Instruction * Inst = dyn_cast<Instruction>(U)) {
        if (Inst->getParent() != Body) {
          auto arrayPtr = createArray(F, I.getType(), NewLoop->getLoopTripCount());

          IRBuilder<> Builder(F->getContext());
          // Store in temporary variable
          Builder.SetInsertPoint(I.getNextNode());
          auto ptr = Builder.CreateGEP(arrayPtr, {Builder.getInt64(0), NewLoop->getIndexVariable()});
          Builder.CreateStore(&I, ptr);

          Builder.SetInsertPoint(Inst);
          auto * PN = dyn_cast<PHINode>(Inst);
          unsigned i = 0;
          if (PN) {
            for (; i < PN->getNumIncomingValues(); ++i) {
              if (PN->getIncomingValue(i) == &I) {
                Builder.SetInsertPoint(PN->getIncomingBlock(i)->getTerminator());
                break;
              }
            }
          } 
          ptr = Builder.CreateGEP(arrayPtr, {Builder.getInt64(0), OldIndex});
          PN->setIncomingValue(i, Builder.CreateLoad(ptr));
        }
      }
    }
  }
}

void LoopSplitter::findVariableUsePoints() {
  LoadInst * LastLoadI = nullptr;
  for (auto * V : AnnotatedVariables) {
    for (auto * U : V->users()) {
      if (auto * I = dyn_cast<LoadInst>(U)) {
        LastLoadI = I;
      }
    }
    if (LastLoadI) {
      AnnotatedVariableDefPoints.push_back(LastLoadI);
    }
  }
}

}
