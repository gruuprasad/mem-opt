#ifndef TAS_CACHEUSAGEANALYSIS_HPP
#define TAS_CACHEUSAGEANALYSIS_HPP

#include <string>

#include <llvm/ADT/SmallVector.h>
#include <llvm/Analysis/LoopInfo.h>
#include <llvm/Analysis/MemorySSA.h>
#include <llvm/IR/Dominators.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Value.h>

namespace tas {

/// This holds the result of cache analysis performed on a function.
/// Currently the only result data we have is the usage of number of
/// cache lines.
/// FIXME How to properly use this kind of result data along with
/// pass manager?
struct CAResult {
  unsigned NumOfCacheLines; //< Estimated number of cache lines.
  CAResult() = default;
  CAResult(unsigned N) : NumOfCacheLines(N) {}
};

llvm::raw_ostream &operator<<(llvm::raw_ostream &OS, CAResult AR);

/// \param[out] NumOfCacheLines Result of the cache analysis
class CacheUsageAnalysis {
  llvm::Function * F;
  const llvm::DataLayout * DL;
  llvm::DominatorTree DT;
  llvm::LoopInfo LI;
  llvm::SmallVector<const llvm::AllocaInst *, 4> PtrAllocas;
  unsigned CacheLineSize;
  unsigned NumOfCacheLines = 0;
  bool LoopExists = false;

  public:
  CacheUsageAnalysis(llvm::Function * F, unsigned N = 64)
    : F(F), DL(&F->getParent()->getDataLayout()),
      DT(llvm::DominatorTree(*F)), LI(llvm::LoopInfo(DT)),
      CacheLineSize(N) {}

  bool run();
  unsigned getNumOfCacheLines() {
    return NumOfCacheLines;
  }

  /// Hand off cache analysis result
  CAResult getResult() {
    return CAResult(NumOfCacheLines);
  }

  unsigned getByteOffsetRelative(llvm::Type *, unsigned FieldIdx);
  unsigned getByteOffsetAbsolute(const llvm::GetElementPtrInst * BasePtr, unsigned CurOffset);
}; // tas namespace

}

#endif
