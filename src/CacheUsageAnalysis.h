#ifndef TAS_CACHEUSAGEANALYSIS_HPP
#define TAS_CACHEUSAGEANALYSIS_HPP

#include <string>

#include <llvm/Analysis/LoopInfo.h>
#include <llvm/Analysis/MemorySSA.h>
#include <llvm/IR/Dominators.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Value.h>

namespace tas {

class CacheUsageAnalysis {
  llvm::Function * F;
  llvm::LoopInfo * LI;
  llvm::DominatorTree * DT;
  llvm::MemorySSA * MSSA;

  public:
  CacheUsageAnalysis(llvm::Function * F_, llvm::LoopInfo * LI_, llvm::DominatorTree * DT_,
                     llvm::MemorySSA * MSSA) :
    F(F_), LI(LI_), DT(DT_), MSSA(MSSA) {}

  bool run();
}; // tas namespace

}

#endif
