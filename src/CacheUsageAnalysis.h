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
  unsigned NumOfCacheLines = 0;

  public:
  CacheUsageAnalysis(llvm::Function * F)
    : F(F) {}

  bool run();
  unsigned getNumOfCacheLines() {
    return NumOfCacheLines;
  }
  unsigned getCacheLineIdx(llvm::Type *, unsigned FieldIdx);
}; // tas namespace

}

#endif
