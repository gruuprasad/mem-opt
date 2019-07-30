#include "CacheUsageInfo.h"
#include "Macros.h"
#include "Util.h"

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/Statistic.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Support/Debug.h>

#include <algorithm>
#include <string>

using namespace llvm;

#define DEBUG_TYPE "tas"

namespace tas {

unsigned CacheUsageInfo::getByteOffsetRelative(Type * Ty, unsigned FieldIdx) {
  if (Ty->isSingleValueType())
    return 0;

  // Struct type
  if (Ty->isStructTy()) {
    LLVM_DEBUG(errs() << "In getByteOffsetRelative() struct name = " << Ty->getStructName() << "\n");
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
unsigned CacheUsageInfo::getByteOffsetAbsolute(const GetElementPtrInst * CurGEP, unsigned CurOffset) {
  // XXX Assume 1-D compound type.
  auto FieldIdx = getGEPIndex(CurGEP);
  auto RelOffset = getByteOffsetRelative(CurGEP->getSourceElementType(), FieldIdx);
  LLVM_DEBUG(errs() << "CurGEP = " << *CurGEP << " Field Idx = " <<  FieldIdx << " RelOffset = " << RelOffset << "\n");
  CurOffset += RelOffset;

  if (!isa<GetElementPtrInst>(CurGEP->getNextNode()))
    return CurOffset;

  return getByteOffsetAbsolute(cast<GetElementPtrInst>(CurGEP->getNextNode()), CurOffset);
}

bool CacheUsageInfo::analyze(Function & F) {

 return false;
}

} // tas namespace
