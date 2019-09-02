#include "BatchProcess/BatchProcess.h"
#include "BatchProcessPass.h"
#include "Common/Util.h"

#include <llvm/Transforms/Utils.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/IR/Dominators.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/PassRegistry.h>
#include <llvm/Transforms/IPO/PassManagerBuilder.h>

using namespace llvm;

static const std::string fn_mark = "tas_batch";

namespace {

void TASBatchProcess::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.addRequired<DominatorTreeWrapperPass>();
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

  errs() << "BatchProcess pass: " << F.getName() << "\n";
  LoopInfo &LI = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();
  DominatorTree &DT = getAnalysis<DominatorTreeWrapperPass>().getDomTree();
  tas::BatchProcess BP(&F, &LI, &DT);
  return BP.run();
}

char TASBatchProcess::ID = 0;
static RegisterPass<TASBatchProcess> X("tas-batch-process", "Pass to convert sequential process to batch process of packets",
                                   false,
                                     false);
} // Anonymous namespace

static void registerTASPass(const PassManagerBuilder & Builder,
                           legacy::PassManagerBase &PM) {
  PM.add(createIndVarSimplifyPass());        // Canonicalize indvars
  PM.add(createLoopSimplifyPass());          // Loop simplify
  PM.add(createLCSSAPass());
  PM.add(new TASBatchProcess());
}

static RegisterStandardPasses
    RegisterTASPass(PassManagerBuilder::EP_EnabledOnOptLevel0,
                   registerTASPass);
