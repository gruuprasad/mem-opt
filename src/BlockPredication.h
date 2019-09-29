#ifndef BLOCK_PREDICATION_H
#define BLOCK_PREDICATION_H

#include "PacketPathAnalysis.h"

#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/IRBuilder.h>

#include <deque>

namespace tas {

// This class transforms the function in place.
class BlockPredication {
  using BlockToIntMapType = llvm::DenseMap<llvm::BasicBlock *, unsigned>;

  llvm::Function * F;
  PacketPathAnalysis PPA;
  llvm::BasicBlock * EntryBlock;
  llvm::AllocaInst * PathIdAlloca;
  llvm::IRBuilder<> Builder;
  std::deque<llvm::BasicBlock *> ActionBlocks;
  std::deque<llvm::BasicBlock *> PredicateBlocks;

  void linearizeControlFlow();
  void setPathIDCondition(llvm::BranchInst * BI, BlockToIntMapType & PathIDMap);
  void setActionBlocksSuccessors();
  llvm::BasicBlock * insertPredicateBlock(llvm::BasicBlock * ActionBB,
                                          unsigned PathID);
  void setPredicateBlocksFalseEdges();

public:
  BlockPredication(llvm::Function * F_)
    : F(F_), PPA(F), EntryBlock(&F->getEntryBlock()),
      Builder(F->getContext()) {}

  bool run();
  PacketPathAnalysis & getPathAnalysis() { return PPA; };
};

}

#endif
