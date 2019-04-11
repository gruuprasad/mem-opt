#include <llvm/Analysis/LoopInfo.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/Module.h>
#include <llvm/Pass.h>
#include <llvm/Support/Debug.h>
#include <llvm/Support/raw_ostream.h>

#include <cassert>

#include "LoopFission.h"
#include "Util.h"

using namespace llvm;

const std::string fn_mark = "tas_batch";

namespace tas {

static std::string VarAnnotationStr = "llvm.var.annotation";

bool LoopFission::ArePreconditionsMet() {
  return !(LI->empty());
}

void detectPrefetchVariables(Function * F, SmallVectorImpl<Value *> & EI) {
  /*
  for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I) {
    auto * CI = dyn_cast<CallInst>(&*I);
    if(!CI) continue;

    if (CI->getCalledFunction()->getName() != VarAnnotationStr)
      continue;

    auto * ExpensivePtr = (cast<BitCastInst>(CI->getOperand(0)))->getOperand(0);
    EI.push_back(ExpensivePtr);
  }
  */
}

int detectLoopTripCount() {
  return 0;
}

Value * detectPrefetchVariableDef(Value * PV) {
  return nullptr;
}

void collectPrefetchVariablesUses(SmallVectorImpl<Value *> & PrefetchUses,
                                  SmallVectorImpl<Value *> & PrefetechVariables) {
  return;
}

bool LoopFission::run() {
  if (!ArePreconditionsMet())
    return false;

  // Step 1:
  SmallVector<Value *, 8> PrefetechVariables;
  detectPrefetchVariables(F, PrefetechVariables);

  // If there are no annotations, then do nothing for now
  if (PrefetechVariables.empty())
    return false;

  // Step 2:
  // If loop trip count is not constant, then return -1.
  // FIXME In that case loop body is copied transparently
  int LoopTripCount = 0;
  LoopTripCount = detectLoopTripCount();

  // Step 3:
  SmallVector<Value *, 8> PrefetechVariablesDefs;
  for (auto & PV : PrefetechVariables) {
    PrefetechVariablesDefs.push_back(detectPrefetchVariableDef(PV));
  }

  // Step 4:
  SmallVector<Value *, 8> PrefetchUses;
  collectPrefetchVariablesUses(PrefetchUses, PrefetechVariables);

  /*
  for (auto & SP : SplitPoints)
    SP->getParent()->splitBasicBlock(SP, "new_BB");

  // Assume there is only single top-level loop in this function.
  auto L = *LI->begin();

  LLVM_DEBUG(dbgs() << "Function before loop clone:\nNumber of BasicBlock:" << F->size() << "\n" << *F);
  ValueToValueMapTy VMap;
  cloneLoopBasicBlocks(F, L, VMap);
  LLVM_DEBUG(dbgs() << "Function after loop clone:\nNumber of BasicBlock:" << F->size() << "\n" << *F);
  */
  return false;
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

  LLVM_DEBUG(errs() << "LoopFission pass: " << F.getName() << "\n");
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
