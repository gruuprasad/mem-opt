#ifndef BLOCK_PREDICATION_H
#define BLOCK_PREDICATION_H

#include "PacketPathAnalysis/PacketPathAnalysis.h"

#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>

namespace tas {

// This class transforms the function in place.
class BlockPredication {
  llvm::Function * F;
  PacketPathAnalysis PPA;

  void savePathState(llvm::BranchInst * BI, llvm::AllocaInst * AI);

public:
  BlockPredication(llvm::Function * F_) : F(F_), PPA(F) {}

  bool run();

  void setPathStateForEachCondBranch();
};

}

#endif
