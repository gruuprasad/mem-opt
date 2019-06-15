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

  public:
  BatchMaker(llvm::Function * F_) :
    F(F_) {}

  bool run();
  void makeFnPrototypeBatchedForm();

}; // tas namespace

}

#endif
