#include "CacheUsageAnalysis.h"
#include "Macros.h"
#include "Util.h"

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/Statistic.h>
#include <llvm/IR/IRBuilder.h>

#include <algorithm>
#include <string>

using namespace llvm;

#define DEBUG_TYPE "tas-cache-analysis"

namespace tas {

unsigned CacheUsageAnalysis::getByteOffsetRelative(Type * Ty, unsigned FieldIdx) {
  if (Ty->isSingleValueType())
    return 0;

  // Struct type
  if (Ty->isStructTy()) {
    //errs() << "In getByteOffsetRelative() struct name = " << Ty->getStructName() << "\n";
    auto * TyLayout = DL->getStructLayout(cast<StructType>(Ty));
    return TyLayout->getElementOffset(FieldIdx);
  }

  // Array type
  return DL->getTypeAllocSize(cast<ArrayType>(Ty)->getArrayElementType()) * FieldIdx;
}

/* This method recursively computes the byte offset of the field to be accessed.
 * In a sequence of GEP instruction, for each instruction relative offset is
 * computed and accumulated.
 */
unsigned CacheUsageAnalysis::getByteOffsetAbsolute(const GetElementPtrInst * CurGEP, unsigned CurOffset) {
  // XXX Assume 1-D compound type.
  auto FieldIdx = getGEPIndex(CurGEP);
  auto RelOffset = getByteOffsetRelative(CurGEP->getSourceElementType(), FieldIdx);
  //errs() << "CurGEP = " << *CurGEP << " Field Idx = " <<  FieldIdx << " RelOffset = " << RelOffset << "\n";
  CurOffset += RelOffset;

  if (!isa<GetElementPtrInst>(CurGEP->getNextNode()))
    return CurOffset;

  return getByteOffsetAbsolute(cast<GetElementPtrInst>(CurGEP->getNextNode()), CurOffset);
}

bool CacheUsageAnalysis::run() {
  using BasePtrType = std::pair<const llvm::Value *, unsigned>;

  // Each entry is unique and non-aliasing
  DenseSet<std::pair<BasePtrType, unsigned>> MemoryCacheLineId;

  // Collect Alloca variable with content as pointer type.
  // We don't track explicitely stack allocated memory for now.
  for (const auto & I : F->getEntryBlock()) {
    if (auto * Alloca = dyn_cast<AllocaInst>(&I)) {
      auto * Pointee = Alloca->getAllocatedType();
      if (!Pointee->isPointerTy()) continue;
      if (Pointee->getPointerElementType()->isSingleValueType()) continue;
      PtrAllocas.push_back(Alloca);
    }
  }

  for (const auto & Alloca : PtrAllocas) {
    // Each Memory Object having compound data type like struct, its size
    // can be represented in terms of number of cache lines i.e suppose all 
    // fields of the compound types are accessed, then how many cache lines
    // taken to keep the data in cache.
    //
    // Suppose field A to be accessed is already present in cache then it is
    // cache hit. This can happen if field B 
    // * is accessed previously.
    // * it belongs to the same cache line as A.
    // * cache line is not replaced. (XXX We assume this condition met always).

    for (auto * U : Alloca->users()) {
      if (!isa<LoadInst>(U)) continue;
      const Instruction * BasePtr = cast<LoadInst>(U);
      BasePtrType Key = std::make_pair(Alloca, 0);

      // Check whether ptr is double pointer, then update key and baseptr
      // XXX Bit hacky, any elegant way?
      if (Alloca->getAllocatedType()->getPointerElementType()->isPointerTy()) {
        Key = std::make_pair(Alloca, getGEPIndex(cast<GetElementPtrInst>(BasePtr->getNextNode())));
        BasePtr =  BasePtr->getNextNode()->getNextNode();
      }

      //errs() << "\nKey = " << *Key.first << " " << Key.second << "\n";
      //errs() << "Use = " << *U << "\n";
      if (!isa<GetElementPtrInst>(BasePtr->getNextNode())) continue;

      unsigned AbsoluteOffset = getByteOffsetAbsolute(cast<GetElementPtrInst>(BasePtr->getNextNode()), 0 /*Base Address idx */);
      //errs() << "Absolute Offset = " << AbsoluteOffset 
      //  << " Cacheline " << AbsoluteOffset/CACHELINESIZE_BYTES << "\n";
      MemoryCacheLineId.insert(std::make_pair(Key, AbsoluteOffset/CACHELINESIZE_BYTES));
    }
  }

  // total number of cache lines tracked.
  NumOfCacheLines = MemoryCacheLineId.size();

 return false;
}

} // tas namespace
