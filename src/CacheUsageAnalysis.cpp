#include "CacheUsageAnalysis.h"
#include "Macros.h"

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/Statistic.h>
#include <llvm/IR/IRBuilder.h>

#include <algorithm>
#include <string>

using namespace llvm;

#define DEBUG_TYPE "tas-cache-analysis"

namespace tas {

unsigned CacheUsageAnalysis::getCacheLineIdx(Type * Ty, unsigned FieldIdx) {
  auto & DL = F->getParent()->getDataLayout();

  StructType * StructTy;
  if (Ty->isStructTy())
    StructTy = cast<StructType>(Ty);
  else
    return 0;
  auto * TyLayout = DL.getStructLayout(StructTy);
  return std::floor(TyLayout->getElementOffset(FieldIdx) / CACHELINESIZE_BYTES);
}

bool CacheUsageAnalysis::run() {

  // Each entry is unique and non-aliasing
  DenseMap<const Value *, SmallVector<unsigned, 4>> MemoryLocValueToCacheIdx;
  
  // Collect Alloca variable with content as pointer type.
  // We don't track explicitely stack allocated memory for now.
  for (const auto & I : F->getEntryBlock()) {
    if (auto * Alloca = dyn_cast<AllocaInst>(&I)) {
      auto * Pointee = Alloca->getAllocatedType();
      if (!Pointee->isPointerTy()) continue;

      // Each Memory Object having compound data type like struct, its size
      // can be represented in terms of number of cache lines i.e suppose all 
      // fields of the compound types are accessed, then how many cache lines
      // taken to keep the data in cache.
      //
      // Suppose field A to be accessed is already present in cache then it is
      // cache hit. This can happen if say, field B 
      // * is accessed previously.
      // * it belongs to the same cache line as A.
      // * cache line is not replaced. (XXX We assume this condition met always).
      // Hence we track each cache line number already accessed for a given
      // Memory Object.
      
      for (auto * U : Alloca->users()) {
        if (!isa<LoadInst>(U) || 
            !isa<GetElementPtrInst>(cast<Instruction>(U)->getNextNode())) continue;

        auto GEPInst = cast<GetElementPtrInst>(cast<Instruction>(U)->getNextNode());
        // XXX Assume 1-D compound type.
        unsigned FieldIdx = 0;
        if (auto * CI = dyn_cast<ConstantInt>(GEPInst->getOperand(GEPInst->getNumIndices()))) {
          FieldIdx = CI->getZExtValue();
        } else {
          assert (0 && "Value has to be constant expression!");
        }
        auto CacheLineIdx = getCacheLineIdx(Pointee->getPointerElementType(), FieldIdx);
        auto & UsedCacheLines = MemoryLocValueToCacheIdx[Alloca];
        auto Result = std::find(UsedCacheLines.begin(), UsedCacheLines.end(), CacheLineIdx);
        if (Result == std::end(UsedCacheLines)) {
          UsedCacheLines.push_back(CacheLineIdx);
        }
      }
    }
  }

  // Sum total number of cache lines tracked.
  NumOfCacheLines = 0;
  for (auto & KV : MemoryLocValueToCacheIdx) {
    NumOfCacheLines += KV.getSecond().size();
  }

 return false;
}

}
