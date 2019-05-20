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
      if (Callee->isIntrinsic()) {
        AnnotatedVariables.push_back(cast<BitCastInst>(CI->getArgOperand(0))->getOperand(0));
      }
    }
  }

  errs() << "Number of annotated Variables: " << AnnotatedVariables.size() << "\n";

  // Insert Prefetch call.
  for (auto & V : AnnotatedVariables) {
    for (auto * U : V->users()) {
      if (auto * ST = dyn_cast<StoreInst>(U)) {

        // Set prefetch instruction insertion point.
        IRBuilder<> Builder(F->getContext());
        Builder.SetInsertPoint(ST->getNextNode());

        // Cast pointer value to i8* type
        auto * PtrVal = cast<Instruction>(ST->getOperand(0));
        auto CastI = Builder.CreateBitCast(PtrVal, Builder.getInt8PtrTy(), "TAS-inst1");

        // Add llvm prefetch intrinsic call.
        Type *I32 = Type::getInt32Ty(F->getContext());
        Value *PrefetchFunc = Intrinsic::getDeclaration(F->getParent(), Intrinsic::prefetch);
        Builder.CreateCall(
            PrefetchFunc,
            {CastI, /* Pointer Value */
            ConstantInt::get(I32, 0), /* read (0) or write (1) */
            ConstantInt::get(I32, 3), /* no_locality (0) to extreme temporal locality (3) */
            ConstantInt::get(I32, 1)} /* data (1) or instruction (0)*/
            );
      }
    }
  }

  return true;
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
