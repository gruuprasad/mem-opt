#ifndef BLOCK_PREDICATION_H
#define BLOCK_PREDICATION_H

#include "PacketPathAnalysis/PacketPathAnalysis.h"

#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/IRBuilder.h>

namespace tas {

class TasException {
public:
  llvm::Function * F_state;
  TasException(llvm::Function * F_) : F_state(F_) {}
  void dump() { F_state->print(llvm::errs()); }
};

// This class transforms the function in place.
class BlockPredication {
  using BlockIDPairType = std::pair<llvm::BasicBlock *, unsigned>;
  llvm::Function * F;
  PacketPathAnalysis PPA;
  llvm::AllocaInst * PathIdAlloca;
  llvm::IRBuilder<> Builder;
  llvm::SmallVector<llvm::BasicBlock *, 16> BlockExecutionOrder;

  void flattenConditionBranchPaths(llvm::BranchInst * BI);
  void linearizeControlFlow();
  llvm::BasicBlock * insertPredicateBlock(BlockIDPairType ActionBB, llvm::BasicBlock * SuccBB);
  llvm::BasicBlock * predicateIfElseBlock(BlockIDPairType IfBB, BlockIDPairType ElseBB);

public:
  BlockPredication(llvm::Function * F_)
    : F(F_), PPA(F), Builder(F->getContext()) {}

  bool run();
};

}

#endif
