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
  //AU.addRequired<DominatorTreeWrapperPass>();
  //AU.setPreservesAll();
}

bool TASBatchMaker::runOnModule(Module &M) {

  errs() << "BatchMaker pass: " << M.getSourceFileName() << "\n";
  DenseMap<Function *, StringRef> AnnotatedFnList;
  tas::getAnnotatedFunctionList(&M, AnnotatedFnList);

  bool changed = false;
  for (auto & FnStr : AnnotatedFnList) {
    if (FnStr.getSecond().compare("tas_batch_maker") != 0) continue;
    tas::BatchMaker BM(FnStr.getFirst());
    changed |= BM.run();
  }
  return changed;
}

char TASBatchMaker::ID = 0;
static RegisterPass<TASBatchMaker> X("tas-batch-maker", "Pass to make batched packet processing",
                                   false,
                                     false);
} // Anonymous namespace

/*
static void registerTASPass(const PassManagerBuilder & Builder,
                           legacy::PassManagerBase &PM) {
  PM.add(new TASBatchMaker());
}

static RegisterStandardPasses
    RegisterTASPass(PassManagerBuilder::EP_EnabledOnOptLevel0,
                   registerTASPass);
                   */

namespace tas {

void runBatchMakerPass(Module * M) {
  legacy::PassManager FPM;
  FPM.add(new DominatorTreeWrapperPass());
  FPM.add(new LoopInfoWrapperPass());
  FPM.add(new TASBatchMaker());
  FPM.run(*M);
}

}
