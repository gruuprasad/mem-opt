#ifndef BLOCK_PREDICATION_H
#define BLOCK_PREDICATION_H

#include "PacketPathAnalysis.h"

#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/IRBuilder.h>

#include <deque>

namespace tas {

// Linerize the control flow in a function.
class BlockPredication {
  using BlockToIntMapType = llvm::DenseMap<llvm::BasicBlock *, unsigned>;

  llvm::Function * F;
  PacketPathAnalysis PPA;
  llvm::BasicBlock * EntryBlock;
  llvm::AllocaInst * MaskIDAlloca;
  llvm::IRBuilder<> Builder;
  std::deque<llvm::BasicBlock *> ActionBlocks;
  std::deque<llvm::BasicBlock *> PredicateBlocks;
  llvm::BasicBlock * ReturnBlock;

  void linearizeControlFlow();
  void setPathIDCondition(llvm::BranchInst * BI, BlockToIntMapType & MaskIDMap);
  void setActionBlocksSuccessors(std::deque<llvm::BasicBlock *> & PredicateBlocks);
  void insertPredicateBlocks();
  void movePhiNodeToPredicateBlock(llvm::BasicBlock * PredBB,
                                   llvm::BasicBlock * ActionBB);
  void setPredBlockSuccessors(std::deque<llvm::BasicBlock *> & PredBlocks,
                                    std::deque<llvm::BasicBlock *> & ActionBlocks);

public:
  BlockPredication(llvm::Function * F_)
    : F(F_), PPA(F), EntryBlock(&F->getEntryBlock()),
      Builder(F->getContext()) {}

  bool run();
  PacketPathAnalysis & getPathAnalysis() { return PPA; };
};

}

#endif
