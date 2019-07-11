#include "CacheUsageAnalysisPass.h"
#include "CacheUsageAnalysis.h"
#include "Util.h"

#include <llvm/Analysis/MemorySSA.h>
#include <llvm/Transforms/Utils.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/IR/Dominators.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/PassRegistry.h>
#include <llvm/Transforms/IPO/PassManagerBuilder.h>

using namespace llvm;

static const std::string fn_mark = "tas_cacheline_count";

namespace {

void CacheUsageAnalysisPass::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.addRequired<DominatorTreeWrapperPass>();
  AU.addRequired<LoopInfoWrapperPass>();
  AU.addRequired<MemorySSAWrapperPass>();
  AU.setPreservesAll();
}

bool CacheUsageAnalysisPass::doInitialization(Module &M) {
  tas::setAnnotationInFunctionObject(&M);
  return true;
}

bool CacheUsageAnalysisPass::runOnFunction(Function &F) {
  if (!F.hasFnAttribute(fn_mark)) 
    return false;

  errs() << "CacheUsageAnalysisPass pass: " << F.getName() << "\n";
  LoopInfo &LI = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();
  DominatorTree &DT = getAnalysis<DominatorTreeWrapperPass>().getDomTree();
  MemorySSA &MSSA = getAnalysis<MemorySSAWrapperPass>().getMSSA();
  return false;
}

char CacheUsageAnalysisPass::ID = 0;
static RegisterPass<CacheUsageAnalysisPass> X("cache-usage-analysis", "Pass to analyze cache usage", false, false);
} // Anonymous namespace

static void registerTASPass(const PassManagerBuilder & Builder,
                           legacy::PassManagerBase &PM) {
  PM.add(new CacheUsageAnalysisPass());
}

static RegisterStandardPasses
    RegisterTASPass(PassManagerBuilder::EP_EnabledOnOptLevel0,
                   registerTASPass);