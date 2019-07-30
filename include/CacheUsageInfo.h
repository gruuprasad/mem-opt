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

/// \param [in] Cache line size in bytes (default = 64)
/// \param[out] NumOfCacheLines Result of the cache analysis
class CacheUsageInfo {
  llvm::Function * F;
  const llvm::DataLayout * DL;
  llvm::DominatorTree DT;
  llvm::LoopInfo LI;
  llvm::SmallVector<const llvm::AllocaInst *, 4> PtrAllocas;
  unsigned CacheLineSize;
  unsigned NumOfCacheLines = 0;

  public:
  CacheUsageInfo() = default;
  CacheUsageInfo(unsigned N) : CacheLineSize(N) {}
  CacheUsageInfo(llvm::Function * F, unsigned N = 64)
    : F(F), DL(&F->getParent()->getDataLayout()),
      DT(llvm::DominatorTree(*F)), LI(llvm::LoopInfo(DT)),
      CacheLineSize(N) {}

  bool analyze(llvm::Function & F);
  unsigned getNumOfCacheLines() { return NumOfCacheLines; }
  unsigned getByteOffsetRelative(llvm::Type *, unsigned FieldIdx);
  unsigned getByteOffsetAbsolute(const llvm::GetElementPtrInst * BasePtr, unsigned CurOffset);
}; // tas namespace

}

#endif
