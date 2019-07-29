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

namespace tas {

void CacheUsageAnalysisPass::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.addRequired<DominatorTreeWrapperPass>();
  AU.addRequired<LoopInfoWrapperPass>();
  AU.setPreservesAll();
}

/// Extract function annotation info from llvm.var.annotations in Module object
/// if any and add it as Attribute to Function of interest.
bool CacheUsageAnalysisPass::doInitialization(Module &M) {
  tas::setAnnotationInFunctionObject(&M);
  return true;
}

bool CacheUsageAnalysisPass::runOnFunction(Function &F) {
  if (!F.hasFnAttribute(fn_mark)) 
    return false;

  LoopInfo &LI = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();
  DominatorTree &DT = getAnalysis<DominatorTreeWrapperPass>().getDomTree();
  CacheUsageAnalysis CA (&F, CacheLineSize);
  CA.run();
  errs() << "CacheUsageAnalysisPass pass: " << F.getName() << "\n";
  errs() << "Cachelines needed = " << CA.getNumOfCacheLines() << "\n";
  return false;
}

char CacheUsageAnalysisPass::ID = 0;
static RegisterPass<CacheUsageAnalysisPass> X("cache-usage-analysis", "Pass to analyze cache usage", false, false);

} // tas namespace

static void registerTASPass(const PassManagerBuilder & Builder,
                           legacy::PassManagerBase &PM) {
  PM.add(new tas::CacheUsageAnalysisPass());
}

/*
static RegisterStandardPasses
    RegisterTASPass(PassManagerBuilder::EP_EnabledOnOptLevel0,
                   registerTASPass);
                   */
