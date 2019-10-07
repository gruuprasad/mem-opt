#include "LoopSplitter.h"
#include "ForLoop.h"
#include "Util.h"

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/IRBuilder.h>

#include <iostream>
#include <string>

using namespace llvm;

#define DEBUG_TYPE "tas-batch-process"

namespace tas {

auto detectExpPtrUses(SmallVectorImpl<Value *> & AnnotatedVars) {
  SmallVector<LoadInst *, 4> VarUsePoints;
  for_each(AnnotatedVars, [&]
      (const auto & Var) { 
        auto FU = findFirstUseOfValueInInstType<LoadInst>(Var);
        if (!FU) return;
        // TODO Assume Load instruction immeditately follows.
        if (auto * Ptr = dyn_cast<LoadInst>(FU->getNextNode())) {
          if (Ptr->getOperand(0) == FU)
          VarUsePoints.push_back(const_cast<LoadInst *>(FU));
        }
      });
  return VarUsePoints;
}

bool LoopSplitter::run() {
  auto AnnotatedVars = detectExpPtrVars(F);

  auto VarUsePoints = detectExpPtrUses(AnnotatedVars);

  for_each(VarUsePoints,
      [&] (auto & VarUse) { insertLLVMPrefetchIntrinsic(F, VarUse); });
  
  stat = Stats(AnnotatedVars.size(), VarUsePoints.size());

  return true;
}

}
