#include "CacheUsageAnalysisPass.h"
#include "CacheAnalysis/CacheUsageInfo.h"
#include "Common/Util.h"

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

llvm::AnalysisKey CacheUsageAnalysis::Key;

tas::CacheUsageInfo CacheUsageAnalysis::run(llvm::Function &F, llvm::FunctionAnalysisManager &AM) {
  auto & DT = AM.getResult<DominatorTreeAnalysis>(F);
  auto & LI = AM.getResult<LoopAnalysis>(F);
  tas::CacheUsageInfo CI(CacheLineSize);
  CI.analyze(F);
  return CI;
}

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
  CI.analyze(F);
  return false;
}

char CacheUsageAnalysisPass::ID = 0;
static RegisterPass<CacheUsageAnalysisPass> X("cache-usage-analysis", "Pass to analyze cache usage", false, false);

void runCacheAnalysisPass(Module * M, unsigned CacheLineSize) {
  legacy::PassManager FPM;
  FPM.add(new DominatorTreeWrapperPass());
  FPM.add(new LoopInfoWrapperPass());
  FPM.add(new tas::CacheUsageAnalysisPass(CacheLineSize));
  FPM.run(*M);

  /*
  FunctionAnalysisManager FAM (true);
  FAM.registerPass([&] { return tas::CacheUsageAnalysis(CacheLineSize); });
  FunctionPassManager FPM;

  ModuleAnalysisManager MAM (true);
  MAM.registerPass([&] { return FunctionAnalysisManagerModuleProxy(FAM); });

  ModulePassManager MPM;
  MPM.addPass(createModuleToFunctionPassAdaptor(std::move(FPM)));
  errs() << "Running pass\n";
  MPM.run(*M, MAM);
  */
}

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

