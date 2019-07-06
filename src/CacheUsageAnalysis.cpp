#include "CacheUsageAnalysis.h"

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/Statistic.h>
#include <llvm/IR/IRBuilder.h>

#include <string>

using namespace llvm;

#define DEBUG_TYPE "tas-cache-analysis"

namespace tas {

bool CacheUsageAnalysis::run() {
  return false;
}

}
