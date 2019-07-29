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
  unsigned getByteOffsetRelative(llvm::Type *, unsigned FieldIdx);
  unsigned getByteOffsetAbsolute(const llvm::GetElementPtrInst * BasePtr, unsigned CurOffset);
}; // tas namespace

}

#endif
