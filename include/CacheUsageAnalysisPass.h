#ifndef TAS_CACHEUSAGEANALYSISPASS_H
#define TAS_CACHEUSAGEANALYSISPASS_H

#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/Pass.h>

namespace tas {

struct CacheUsageAnalysisPass : public llvm::FunctionPass {
  static char ID;
  unsigned CacheLineSize;
  CacheUsageAnalysisPass(unsigned N = 64) : FunctionPass(ID), CacheLineSize(N) {}

  void getAnalysisUsage(llvm::AnalysisUsage &AU) const override;
 
  bool doInitialization(llvm::Module &M) override;

  bool runOnFunction(llvm::Function &F) override;
};

}

#endif
