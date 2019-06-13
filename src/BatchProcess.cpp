#include "BatchProcess.h"
#include "ForLoop.h"
#include "Util.h"

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/Statistic.h>
#include <llvm/IR/CFG.h>
#include <llvm/IR/IRBuilder.h>

#include <iostream>
#include <string>

using namespace llvm;

#define DEBUG_TYPE "tas-batch-process"

STATISTIC(NumAnnotatedVariables, "Number of annotated variables detected");
STATISTIC(NumPrefetchInsts, "Number of Prefetch instructions added");

namespace tas {

bool BatchProcess::run() {
/* Pre-condition checks:
 * 1. Function has atleast one loop and loop is in canonical form.
 *
 * Step 1: Detect and store all variables marked with prefetch annotation.
 * Step 2: Store trip count for loop. 
 * Step 3: Find store instructions whose operand is prefetch annotated variables and save them.
 *         These instructions act as loop split boundary.
 * Step 4: Identify variables whose values are defined in one loop and used in other loop. We need to store such variables
 *         in an array of size trip count.
 * Step 5: Replace use of variables in step 4 with indexed array variable.
 * Step 5: Split loop into multiple loops with same trip count.
 * Step 6: Add new loop above first loop, which has prefetch instruction as it's body.
 * Step 7: Handle control flow.
 * Step 8: In each loop insert prefetch instruction for memory access of next loop.
 */

  detectAnnotatedVariable();

  if (AnnotatedVariables.empty())
    return false;

  auto LIt = LI->begin();
  Loop * L0 = *LIt; // XXX Split only first top level loop
  splitLoop(L0);

  //insertPrefetchCalls();
 //F->print(errs());
 
  return true;
}

void BatchProcess::splitLoop(Loop * L0) {
  // Original loop is retained, but it's body is split on each iteration.
  // One part becomes part of the new loop and rest remains with the old loop.
  auto * L0_Head = L0->getHeader(); //Doesn't change.
  auto * L0_IndexVar = L0->getCanonicalInductionVariable();

  //TODO Add check for loop-simplified form.
  // Preheader changes on every new loop insertion
  auto * PreHeader = L0->getLoopPreheader();

  // If there is no phi node, that means loop is not in loop-simplified form.
  // we don't do anything in that case.
  if (!isa<PHINode>(L0->getHeader()->begin()))
      return;

  // Convert scalar alloca variable into array form
  auto tripCount = TASForLoop::getLoopTripCount();
  IRBuilder<> Builder(F->getContext());
  for (auto & AI : AnnotatedVariables) {
    auto arrayPtr = createArray(cast<AllocaInst>(AI)->getAllocatedType(), tripCount);
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
      auto ptr = Builder.CreateGEP(arrayPtr, {Builder.getInt64(0), L0_IndexVar});
      U->replaceUsesOfWith(AI, ptr);
      NumUses--;
    }
  }

  // Remove phi node entries if any
  auto * PN = &*(L0->getHeader()->phis().begin());
  PN->removeIncomingValue(L0->getLoopPreheader());

  // Control flow before loop split op.
  // L0_PreHeader --> [L0_Head --> L0_body-->L0_Latch] --> Block outside loop
  // control flow edges between loops: [L0_PreHeader --> L0_Head] [L0_Head --> Block outside loop].
  // After Loop split op
  // L0_PreHeader --> [L1_PreHeader --> L1_Head -----L1_Latch] --> [L0_Head ------- L0_Latch] 
  // control flow edge between loops: [L0_PreHeader --> L1_preheader] [L1_Head --> L0_Head] [L0_Head --> Block outside loop]
  
  // Split basic block at annotated variable def points.
  unsigned int i = 0;
  auto PA_It= PrefetchAddresses.begin();
  for (auto & DP : AnnotatedVariableDefPoints) {
 
    // Insert new loop
    auto TL0 = TASForLoop(F->getContext(), PreHeader, L0_Head, "tas.loop." + std::to_string(i), F);

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
  // TODO Check whether index variable type matches, otherwise apply cast.
  PN->addIncoming(ConstantInt::get(F->getContext(), APInt(16, 0, true)), PreHeader);
  //F->print(errs());
}

void BatchProcess::fixValueDependenceBetWeenLoops(TASForLoop * NewLoop, Value * OldIndex) {
  BasicBlock * Body = NewLoop->getBody();
  assert (Body != nullptr && "Loop must have a body");
  for (Instruction & I : *Body) {
    for (auto * U : I.users()) {
      if (Instruction * Inst = dyn_cast<Instruction>(U)) {
        if (Inst->getParent() != Body) {
          auto arrayPtr = createArray(I.getType(), NewLoop->getLoopTripCount());

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

Value * BatchProcess::createArray(Type * Ty, unsigned size) {
  // Allocate temporary array
  IRBuilder<> Builder (F->getContext());
  auto BB = F->begin();
  auto TermI = (*BB).getTerminator();
  Builder.SetInsertPoint(TermI);
  return Builder.CreateAlloca(ArrayType::get(Ty, size));
}

void BatchProcess::detectAnnotatedVariable() {
  auto varAnnotationIntrinsic = Function::lookupIntrinsicID("llvm.var.annotation");
  // XXX Checking only entry basic block for annotated variables.
  for (auto & I : F->front()) {
    if (auto * CI = dyn_cast<CallInst>(&I)) {
      auto * Callee = CI->getCalledFunction();
      if (!Callee->isIntrinsic() || Callee->getIntrinsicID() != varAnnotationIntrinsic) continue;

      AnnotatedVariables.push_back(cast<BitCastInst>(CI->getArgOperand(0))->getOperand(0));
      ++NumAnnotatedVariables;
    }
  }
  findVariableUsePoints();
}

void BatchProcess::findVariableUsePoints() {
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

/*
void BatchProcess::insertPrefetchCalls() {
  // Insert Prefetch call.
  for (auto & V : AnnotatedVariables) {
    for (auto * U : V->users()) {
      if (auto * ST = dyn_cast<StoreInst>(U)) {
        insertLLVMPrefetchIntrinsic(F, ST);
      }
    }
  }
}
*/

}
