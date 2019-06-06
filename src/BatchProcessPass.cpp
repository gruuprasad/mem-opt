#include "BatchProcess.h"
#include "BatchProcessPass.h"
#include "Util.h"

#include <llvm/IR/LegacyPassManager.h>
#include <llvm/PassRegistry.h>
#include <llvm/Transforms/IPO/PassManagerBuilder.h>

using namespace llvm;

static const std::string fn_mark = "tas_batch";

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

static void registerTASPass(const PassManagerBuilder &,
                           legacy::PassManagerBase &PM) {
    PM.add(new TASBatchProcess());
}

static RegisterStandardPasses
    RegisterTASPass(PassManagerBuilder::EP_EarlyAsPossible,
                   registerTASPass);
