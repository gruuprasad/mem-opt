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

unsigned CacheUsageAnalysis::getByteOffsetRelative(Type * Ty, unsigned FieldIdx) {
  // XXX Only struct type supported for this analysis for now.
  if (!Ty->isStructTy())
    return 0;

  auto * TyLayout = DL->getStructLayout(cast<StructType>(Ty));
  return TyLayout->getElementOffset(FieldIdx);
}

unsigned CacheUsageAnalysis::getByteOffsetAbsolute(const GetElementPtrInst * CurGEP, unsigned CurOffset) {
  // XXX Assume 1-D compound type.
  unsigned FieldIdx = 0;
  if (auto * CI = dyn_cast<ConstantInt>(CurGEP->getOperand(CurGEP->getNumIndices()))) {
    FieldIdx = CI->getZExtValue();
  } else {
    assert (0 && "Value has to be constant expression!");
  }
  CurOffset += getByteOffsetRelative(CurGEP->getSourceElementType(), FieldIdx);

  if (!isa<GetElementPtrInst>(CurGEP->getNextNode()))
    return CurOffset;

  return getByteOffsetAbsolute(cast<GetElementPtrInst>(CurGEP->getNextNode()), CurOffset);
}

bool CacheUsageAnalysis::run() {

  // Each entry is unique and non-aliasing
  DenseSet<std::pair<const Value *, unsigned>> MemoryCacheLineId;
  
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
      
      for (auto * U : Alloca->users()) {
        if (!isa<LoadInst>(U)) continue;
        auto BasePtr = cast<LoadInst>(U);

        if (!isa<GetElementPtrInst>(BasePtr->getNextNode())) continue;

        unsigned ByteOffset = getByteOffsetAbsolute(cast<GetElementPtrInst>(BasePtr->getNextNode()), 0 /*Base Address idx */);
        /*
        errs() << "Alloca = " << *Alloca << "  " << " Offset = "
               << ByteOffset << " Cacheline " << ByteOffset/CACHELINESIZE_BYTES << "\n";
        */
        MemoryCacheLineId.insert(std::make_pair(Alloca, ByteOffset/CACHELINESIZE_BYTES));
      }
    }
  }

  // total number of cache lines tracked.
  NumOfCacheLines = MemoryCacheLineId.size();

 return false;
}

}
