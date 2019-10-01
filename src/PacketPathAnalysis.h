#ifndef PACKET_PATH_TRACE_ANALYSIS
#define PACKET_PATH_TRACE_ANALYSIS

#include <llvm/Analysis/DominanceFrontier.h>
#include <llvm/Analysis/PostDominators.h>
#include <llvm/Analysis/RegionInfo.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Dominators.h>

namespace tas {

class PacketPathAnalysis {
  using BBToIntType = llvm::DenseMap<llvm::BasicBlock *, unsigned>;

  llvm::Function * F;
  int MaskIDCounter = 0;

  BBToIntType BlockToMaskID;

public:
  PacketPathAnalysis(llvm::Function * F_) :
    F(F_) {
    calculate();
  }

  void calculate();
  void dumpDebugDataToConsole();

  // Accessors
  unsigned getNumerOfPaths() { return MaskIDCounter; }

  BBToIntType getBlockPathCondition() { return BlockToMaskID; }
};

}

#endif
