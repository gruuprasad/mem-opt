#ifndef TAS_FORLOOPV2_H
#define TAS_FORLOOPV2_H

#include <llvm/Analysis/LoopInfo.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>

#include "Util.h"

namespace tas {

// This is similar to Loop object available in LoopInfo object
// but simple.
class IRLoop {
  llvm::LLVMContext & Ctx;
  llvm::BasicBlock * PreHeader;
  llvm::BasicBlock * Header;
  llvm::BasicBlock * Latch;
  llvm::BasicBlock * EmptyBody; // Useful for creating empty loop.
  llvm::AllocaInst * IdxAlloca;
  llvm::SmallVector<llvm::BasicBlock *, 4> Blocks;
  llvm::BasicBlock * ExitBlock;

public:
  IRLoop(llvm::LLVMContext & C) : Ctx(C) {}

  void analyze(llvm::Loop * L);
  void constructEmptyLoop(llvm::AllocaInst * TripCount, llvm::AllocaInst * Idx,
                          llvm::Function * F);
  void extractLoopSkeleton(llvm::Loop * L);
  void setLoopBlocks(std::vector<llvm::BasicBlock *> & Blocks);

  llvm::BasicBlock * getPreHeader() {
    return PreHeader;
  }

  llvm::BasicBlock * getHeader() {
    return Header;
  }

  llvm::BasicBlock * getLatch() {
    return Latch;
  }

  llvm::BasicBlock * getExitBlock() {
    return ExitBlock;
  }

  void setExitBlock(llvm::BasicBlock * ExitBB) {
    setSuccessor(Header, ExitBB, 1);
  }

  bool contains(llvm::BasicBlock * BB) {
    return std::find(Blocks.begin(), Blocks.end(), BB) != Blocks.end();
  }

  void printLooopInfo() {
    llvm::errs() << "LoopInfo:";
    Header->printAsOperand(llvm::errs());
    llvm::errs() << "  ";
    Latch->printAsOperand(llvm::errs());
    llvm::errs() << "  ";
    llvm::errs() << *IdxAlloca << "\n";
    llvm::errs() << " No of Blocks = " << Blocks.size() << "\n";
  }
};

class LoopBodyTraverser {
  llvm::SmallVector<llvm::BasicBlock *, 4> ExitBlocks;

public:
  LoopBodyTraverser(llvm::Loop * L) {
    L->getExitBlocks(ExitBlocks);
  }

  void traverse(std::vector<llvm::BasicBlock *> & Blocks,
                llvm::BasicBlock * Start, llvm::BasicBlock * End);

  void traverseReverse(std::vector<llvm::BasicBlock *> & Blocks,
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
