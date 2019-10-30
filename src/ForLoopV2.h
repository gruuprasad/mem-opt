#ifndef TAS_FORLOOPV2_H
#define TAS_FORLOOPV2_H

#include <llvm/Analysis/LoopInfo.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>

namespace tas {

// This is similar to Loop object available in LoopInfo object
// but simple.
class IRLoop {
  llvm::BasicBlock * PreHeader;
  llvm::BasicBlock * Header;
  llvm::BasicBlock * Latch;
  llvm::BasicBlock * EmptyBody; // Useful for creating empty loop.
  llvm::AllocaInst * IdxAlloca;
  llvm::SmallVector<llvm::BasicBlock *, 4> Blocks;

public:
  IRLoop() = default;

  void analyze(llvm::Loop * L);
  void constructEmptyLoop(llvm::Value * TripCount,
                          llvm::BasicBlock * InsertAfter);
  void extractLoopSkeleton(llvm::Loop * L);
  void setLoopBlocks(llvm::SmallVectorImpl<llvm::BasicBlock *> & Blocks);

  llvm::BasicBlock * getPreHeader() {
    return PreHeader;
  }

  llvm::BasicBlock * getHeader() {
    return Header;
  }
};

class LoopBodyTraverser {
  llvm::SmallVector<llvm::BasicBlock *, 4> ExitBlocks;

public:
  LoopBodyTraverser(llvm::Loop * L) {
    L->getExitBlocks(ExitBlocks);
  }

  void traverse(llvm::SmallVectorImpl<llvm::BasicBlock *> & Blocks,
                llvm::BasicBlock * Start, llvm::BasicBlock * End);

  void printExitBlocks() {
    llvm::errs() << "Exit blocks = ";
    for (auto & BB : ExitBlocks) {
      BB->printAsOperand(llvm::errs());
      llvm::errs() << " ";
    }
    llvm::errs() << "\n";
  }
};

}

#endif
