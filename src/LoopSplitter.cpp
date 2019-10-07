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

void addPrefetchCallForPointers(Function *F, Stats & stat) {
  auto AnnotatedVars = detectExpPtrVars(F);
  auto VarUsePoints = detectExpPtrUses(AnnotatedVars);
  for_each(VarUsePoints,
      [&] (auto & VarUse) { insertLLVMPrefetchIntrinsic(F, VarUse); });

  stat.AnnotatedVarsSize = AnnotatedVars.size();
  stat.VarUsePointsSize = VarUsePoints.size();
}

bool LoopSplitter::run() {
  Stats stat;
  addPrefetchCallForPointers(F, stat);

  return true;
}

}
