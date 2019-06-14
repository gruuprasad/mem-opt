#ifndef TAS_BATCHMAKER_H
#define TAS_BATCHMAKER_H

#include <string>

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/PostOrderIterator.h>
#include <llvm/Analysis/LoopInfo.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>

namespace tas {

class TASForLoop;

class BatchMaker {
  llvm::Function * F;
  llvm::DominatorTree * DT;

  public:
  BatchMaker(llvm::Function * F_, llvm::DominatorTree * DT_) :
    F(F_), DT(DT_) {}

  bool run();

}; // tas namespace

}

#endif
