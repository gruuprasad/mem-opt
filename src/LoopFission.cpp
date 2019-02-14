#include <llvm/Analysis/LoopInfo.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/Module.h>
#include <llvm/Pass.h>
#include <llvm/Support/raw_ostream.h>

#include <cassert>

#include "LoopFission.h"
#include "Util.h"

using namespace llvm;

const std::string fn_mark = "tas_batch";

namespace tas {

bool LoopFission::run() {
  // Assume there is only single top-level loop in this function.
  assert(!LI->empty() && "Function must contain atleast one loop");
  auto L = *LI->begin();

  errs() << "Function before loop clone:\nNumber of BasicBlock:" << F->size() << "\n" << *F;
  ValueToValueMapTy VMap;
  cloneLoopBasicBlocks(F, L, VMap);
  errs() << "Function after loop clone:\nNumber of BasicBlock:" << F->size() << "\n" << *F;
  return true;
}

} // namespace tas

namespace {

void TASLoopFission::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.addRequired<LoopInfoWrapperPass>();
  //AU.setPreservesAll();
}

bool TASLoopFission::doInitialization(Module &M) {
  tas::setAnnotationInFunctionObject(&M);
  return true;
}

// Perform loop-fission on function annotated with tas attribute.
bool TASLoopFission::runOnFunction(Function &F) {
  if (!F.hasFnAttribute(fn_mark)) 
    return false;

  errs() << "LoopFission pass: " << F.getName() << "\n";
  //tas::detectVarAnnotation(&F);
  LoopInfo &LI = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();
  tas::LoopFission LF(&F, &LI);
  return LF.run();
}

char TASLoopFission::ID = 0;
static RegisterPass<TASLoopFission> X("tas-loop-fission", "Pass replicating single loop into multiple instances",
                                   false,
                                   false);
} // Anonymous namespace
