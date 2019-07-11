#include "CacheUsageAnalysis.h"

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/Statistic.h>
#include <llvm/IR/IRBuilder.h>

#include <string>

using namespace llvm;

#define DEBUG_TYPE "tas-cache-analysis"

namespace tas {

bool CacheUsageAnalysis::run() {
 auto Acc =  MSSA->getBlockAccesses(&F->getEntryBlock());
 auto Defs = MSSA->getBlockDefs(&F->getEntryBlock());

 for (auto & D : *Acc) {
   errs() << "Access: ";
   errs() << D << *cast<MemoryUseOrDef>(D).getMemoryInst() << "\n";
 }

 for (auto & D : *Defs) {
   errs() << "Defs: ";
   errs() << D << *cast<MemoryUseOrDef>(D).getMemoryInst() << "\n";
 }

 return false;
}

}
