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

bool LoopSplitter::run() {
  SmallVector<Value *, 4> AnnotatedVars;
  detectExpPtrVars(F, AnnotatedVars);

  SmallVector<const LoadInst *, 4> VarUsePoints;
  for_each(AnnotatedVars, [&]
      (const auto & Var) { 
        auto FU = findFirstUseOfValueInInstType<LoadInst>(Var);
        if (FU) VarUsePoints.push_back(FU);
      });

  stat = Stats(AnnotatedVars.size(), VarUsePoints.size());

  return true;
}

}
