#include "BatchProcess.h"
#include "Util.h"

#include <llvm/IR/IRBuilder.h>

#include <iostream>

using namespace llvm;

static const std::string fn_mark = "tas_batch";

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

  // Step 1
  for (auto & I : F->front()) {
    if (auto * CI = dyn_cast<CallInst>(&I)) {
      auto * Callee = CI->getCalledFunction();
      if (!Callee->isIntrinsic()) continue;

      AnnotatedVariables.push_back(cast<BitCastInst>(CI->getArgOperand(0))->getOperand(0));
      for (auto * U : AnnotatedVariables.back()->users()) {
        if (auto * ST = dyn_cast<StoreInst>(U))
          AnnotatedVariableDefPoints.push_back(ST);
      }
    }
  }

  errs() << "Number of annotated Variables: " << AnnotatedVariables.size() << "\n";
  errs() << "Number of annotated Variables Defs: " << AnnotatedVariableDefPoints.size() << "\n";

  /*
  // Split basic block at annotated variable def points.
  for (auto & DP : AnnotatedVariableDefPoints)
    DP->getParent()->splitBasicBlock(DP->getNextNode(), "batch_edge_0");
    */

  /*
  // Insert Prefetch call.
  for (auto & V : AnnotatedVariables) {
    for (auto * U : V->users()) {
      if (auto * ST = dyn_cast<StoreInst>(U)) {
        insertLLVMPrefetchIntrinsic(F, ST);
      }
    }
  }
  */

  addEmptyLoop(&F->back());


  return true;
}

void BatchProcess::addEmptyLoop(BasicBlock * InsertBefore) {
  auto & Ctx = F->getContext();
  auto * ForLoopPreHeader = BasicBlock::Create(Ctx, "tas.loop.0.preheader", F, InsertBefore);
  auto * ForLoopHeader = BasicBlock::Create(Ctx, "tas.loop.0.header", F, InsertBefore);
  auto * ForLoopLatch = BasicBlock::Create(Ctx, "tas.loop.0.latch", F, InsertBefore);

  InsertBefore->replaceAllUsesWith(ForLoopPreHeader);
  IRBuilder<> Builder(ForLoopPreHeader);
  Builder.CreateBr(ForLoopHeader);

  Builder.SetInsertPoint(ForLoopHeader);
  auto * PN = Builder.CreatePHI(Type::getInt32Ty(Ctx), 2, "indV");

  Builder.SetInsertPoint(ForLoopLatch);
  auto *IVNext = Builder.CreateAdd(PN, Builder.getInt32(1));
  auto * LatchToHeadBr = Builder.CreateBr(ForLoopHeader);

  Builder.SetInsertPoint(ForLoopHeader);
  PN->addIncoming(Builder.getInt32(0), ForLoopPreHeader);
  PN->addIncoming(IVNext, ForLoopLatch);
  auto * icmp = Builder.CreateICmpSLT(PN, Builder.getInt32(32), "loop-predicate");
  Builder.CreateCondBr(icmp, ForLoopLatch, InsertBefore);
}

}

namespace {

void TASBatchProcess::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.addRequired<LoopInfoWrapperPass>();
  //AU.setPreservesAll();
}

bool TASBatchProcess::doInitialization(Module &M) {
  tas::setAnnotationInFunctionObject(&M);
  return true;
}

bool TASBatchProcess::runOnFunction(Function &F) {
  if (!F.hasFnAttribute(fn_mark)) 
    return false;

  LLVM_DEBUG(errs() << "BatchProcess pass: " << F.getName() << "\n");
  LoopInfo &LI = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();
  tas::BatchProcess BP(&F, &LI);
  return BP.run();
}

char TASBatchProcess::ID = 0;
static RegisterPass<TASBatchProcess> X("tas-batch-process", "Pass to convert sequential process to batch process of packets",
                                   false,
                                     false);
} // Anonymous namespace
