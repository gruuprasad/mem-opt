#include "BatchMaker.h"
#include "BatchMakerPass.h"
#include "Util.h"

#include "llvm/Transforms/Utils.h"
#include "llvm/Transforms/Scalar.h"
#include <llvm/IR/Dominators.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/PassRegistry.h>
#include <llvm/Transforms/IPO/PassManagerBuilder.h>

using namespace llvm;

static const std::string fn_mark = "tas_batch_maker";

namespace {

void TASBatchMaker::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.addRequired<DominatorTreeWrapperPass>();
  //AU.setPreservesAll();
}

bool TASBatchMaker::doInitialization(Module &M) {
  tas::setAnnotationInFunctionObject(&M);
  return true;
}

bool TASBatchMaker::runOnFunction(Function &F) {
  if (!F.hasFnAttribute(fn_mark)) 
    return false;

  errs() << "BatchMaker pass: " << F.getName() << "\n";
  DominatorTree &DT = getAnalysis<DominatorTreeWrapperPass>().getDomTree();
  tas::BatchMaker BM(&F, &DT);
  return BM.run();
}

char TASBatchMaker::ID = 0;
static RegisterPass<TASBatchMaker> X("tas-batch-maker", "Pass to make batched packet processing",
                                   false,
                                     false);
} // Anonymous namespace

static void registerTASPass(const PassManagerBuilder & Builder,
                           legacy::PassManagerBase &PM) {
  PM.add(new TASBatchMaker());
}

static RegisterStandardPasses
    RegisterTASPass(PassManagerBuilder::EP_EnabledOnOptLevel0,
                   registerTASPass);
