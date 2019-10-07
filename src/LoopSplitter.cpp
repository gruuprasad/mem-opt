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
  errs() << "No of annotated variables = " << AnnotatedVars.size() << "\n";

  auto FU = findFirstUseOfValueInInstType<LoadInst>(AnnotatedVars.front());

  return true;
}

}
